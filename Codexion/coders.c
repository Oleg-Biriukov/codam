/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/16 17:03:08 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int i = 0;

static void *smth(void *a)
{
	t_coder *array;

	array = (t_coder *) a;
	for(int b = 0; b < 100; b++)
	{
		pthread_mutex_lock((pthread_mutex_t *) array->data->mutex);
		i++;
		pthread_mutex_unlock((pthread_mutex_t *) array->data->mutex);
	}
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
		pthread_create(&data->t, NULL, &smth, array);
	}
	array = la_start(array);
	while (array)
	{
		pthread_join(array->data->t, NULL);
		array = array->next;
	}
	printf("%d", i);
	return (0);
}
