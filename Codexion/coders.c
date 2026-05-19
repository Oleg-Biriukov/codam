/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/19 17:57:10 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int smth(t_array *array)
{
	t_coder			*data;
	struct timeval	current_time;

	data = (t_coder *) array->data;
	gettimeofday(&current_time, NULL);

	while (current_time.tv_sec - data->start.tv_sec < data->t_burnout/1000){
		if (data->is_done == 1)
			return (printf("T%d DONE\n", data->id), 0);
		gettimeofday(&current_time, NULL);
		
	}
	data->is_burnout = 1;
	printf("T%d burn out\n", data->id);
	return (-1);
}


int init_arrays(t_span *s)
{
	pthread_mutex_t mutex;
	t_array	  		*array;
	t_coder   		*data;
	// int 			res[2];

	array = NULL;
	pthread_mutex_init(&mutex, NULL);
	while (la_len(la_start(array)) < s->n_coders){
		data = malloc(sizeof(t_coder));
		array = la_append(array, data);
		if (!array || !data)
			return (-1);
		data->id = la_len(la_start(array)) - 1;
		data->mutex = mutex;
		gettimeofday(&data->start, NULL);
		data->t_compile = s->t_compile;
		data->t_burnout = s->t_burnout;
		data->t_refactor = s->t_refactor;
		data->t_debug = s->t_debug;
		data->is_done = 0;
		data->is_burnout = 0;
		data->compiles = 0;
		pthread_cond_init(&data->cond, NULL);
		
		if (pthread_create(&data->th_burnout, NULL, (void *) &smth, array) != 0 ||
			pthread_create(&data->th_stages, NULL, (void *) &stages, array) != 0)
		{
			printf("Unexpected error");
		}
	}
	s->coders = la_start(array);
	pthread_cond_broadcast(&s->cond);
	return (0);
}
