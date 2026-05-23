/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/23 15:35:12 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// static int proccess(t_array *array)
// {
// 	return (la_len(array));
// 	// t_coder			*data;
// 	// struct timeval	current_time;
// 	// return (0)
// }


int init_arrays(t_span *s)
{
	pthread_mutex_t mutex;
	t_array	  		*array;
	t_coder   		*data;

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
	
		// if (pthread_create(&data->th_burnout, NULL, (void *) &proccess, array) != 0 ||
		// 	pthread_create(&data->th_stages, NULL, (void *) &stages, array) != 0)
		// 	return (-2);
	}
	s->coders = la_start(array);
	return (0);
}
