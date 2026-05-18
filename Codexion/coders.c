/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/18 17:22:59 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void *smth(void *a)
{
	t_array 		*array;
	t_coder			*data;
	struct timeval	current_time;

	array = (t_array *) a;
	data = (t_coder *) array->data;
	gettimeofday(&current_time, NULL);


	while (current_time.tv_sec - data->start.tv_sec != data->t_burnout/1000){
		gettimeofday(&current_time, NULL);
		
	}
	printf("T%d burn out\n", data->id);
	return (NULL);
}


int init_arrays(t_span *s)
{
	t_array	  		*array;
	t_coder   		*data;
	pthread_mutex_t mutex;

	array = NULL;
	pthread_mutex_init(&mutex, NULL);
	while (la_len(la_start(array)) < s->n_coders){
		data = malloc(sizeof(t_coder));
		array = la_append(array, data);
		if (!array || !data)
			return (-1);
		data->id = la_len(la_start(array)) - 1;
		data->mutex = &mutex;
		gettimeofday(&data->start, NULL);
		data->t_compile = s->t_compile;
		data->t_burnout = s->t_burnout;
		data->t_refactor = s->t_refactor;
		data->t_debug = s->t_debug;
		if (pthread_create(&data->t, NULL, &smth, array) != 0)
		{
			printf("Unexpected error");
		}
	}
	array = la_start(array);
	s->coders = array;
	while (array)
	{
		pthread_join(((t_coder *) array->data)->t, NULL);
		array = array->next;
	}
	return (0);
}
