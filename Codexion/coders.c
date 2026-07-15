/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/15 16:51:01 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder(t_coder *data)
{
	t_span  *s;

    s = (t_span *) data->s;
	while(data->compiles != s->n_compiles)
	{
		if (s->is_failed || s->is_over)
			return;
		pthread_mutex_lock(&s->mut);
		while (!(data->conn[0] && data->conn[1]))
			pthread_cond_wait(&data->cond, &s->mut);
		pthread_mutex_unlock(&s->mut);
		printf("[%d ms] Processing of %d coder\n", s->time, data->id);
		sleep(3);
		printf("[%d ms] Done for %d\n", s->time, data->id);
		pthread_mutex_lock(&s->mut);
		data->conn[0]->is_cooldown = 1;
		data->conn[1]->is_cooldown = 1;
		pthread_cond_broadcast(&data->conn[0]->cond);
		pthread_cond_broadcast(&data->conn[1]->cond);
		data->conn[0] = NULL;
		data->conn[1] = NULL;
		data->compiles++;
		pthread_mutex_unlock(&s->mut);
	}
	pthread_mutex_lock(&s->mut);
	data->is_done = 1;
	pthread_mutex_unlock(&s->mut);
}

int init_arrays(t_span *s)
{
	t_array	  		*array;
	t_coder   		*data;

	array = NULL;
	while (la_len(la_start(array)) < s->n_coders){
		data = malloc(sizeof(t_coder));
		array = la_append(array, data);
		if (!array || !data)
			return (-1);
		data->id = la_len(la_start(array));
		pthread_cond_init(&data->cond, NULL);
		gettimeofday(&data->start, NULL);
		gettimeofday(&data->req_t, NULL);
		data->s = (void *) s;
		data->is_done = 0;
		data->is_burnout = 0;
		data->compiles = 0;
		data->conn[0] = NULL;
		data->conn[1] = NULL;
	}
	s->coders = la_start(array);
	return (0);
}
