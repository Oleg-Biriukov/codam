/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/29 16:36:35 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// static int	check_burnout(t_array *array)
// {
// 	pthread_t	t[2];

// 	if (pthread_create(&t[0], NULL, (void *) &cool_down, array) != 0)
// 		return (-1);
// 	if (pthread_create(&t[1], NULL, (void *) &stages, array) != 0)
// 		return (-1);
// 	return (0);
// }

int proccess(t_array *array)
{
	
}


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
		data->id = la_len(la_start(array));
		gettimeofday(&data->start, NULL);
		data->s = (void *) s;
		data->is_done = 0;
		data->is_burnout = 0;
		data->compiles = 0;
		data->conn[0] = NULL;
		data->conn[1] = NULL;
	
		// if (pthread_create(&data->th_burnout, NULL, (void *) &proccess, array) != 0 ||
		// 	pthread_create(&data->th_stages, NULL, (void *) &stages, array) != 0)
		// 	return (-2);
	}
	s->coders = la_start(array);
	return (0);
}
