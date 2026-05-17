/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/17 18:23:01 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void *smth(void *a)
{
	t_coder 		*array;
	struct timeval	current_time;

	array = (t_coder *) a;
	
	while (current_time.tv_sec - array->data->start.tv_sec != array->data->t_burnout/1000){
		gettimeofday(&current_time, NULL);
		
	}
	printf("T%d burn out\n", array->data->id);
	return NULL;
}


int init_coders(t_span *s)
{
	t_coder	  *array;
	t_content *data;

	array = NULL;
	while (la_len(la_start(array)) < s->n_coders){
		data = malloc(sizeof(t_content));
		array = la_append(array, data);
		if (!array || !data)
			return (-1);
		data->id = la_len(la_start(array)) - 1;
		data->mutex = &s->mutex;
		gettimeofday(&data->start, NULL);
		data->t_compile = 2000;
		data->t_burnout = 10000;
		data->t_refactor = 1000;
		data->t_debug = 1500;
		if (pthread_create(&data->t, NULL, &smth, array) != 0)
		{
			printf("Unexpected error");
		}
	}
	array = la_start(array);
	while (array)
	{
		pthread_join(array->data->t, NULL);
		array = array->next;
	}
	return (0);
}
