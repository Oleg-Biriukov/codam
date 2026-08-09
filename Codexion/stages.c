/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stages.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:05:07 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/09 18:18:58 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	start_comp(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut_prnt);
	printf("%d %d is compiling\n", timer(s), data->id);
	pthread_mutex_unlock(&s->mut_prnt);
	pthread_mutex_lock(&s->mut_array);
	data->is_active = false;
	pthread_mutex_unlock(&s->mut_array);
}

void	finish_comp(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut_array);
	if (data->conn[0])
		data->conn[0]->is_cooldown = true;
	if (data->conn[1])
		data->conn[1]->is_cooldown = true;
	if (data->conn[0])
		pthread_cond_broadcast(&data->conn[0]->cond);
	if (data->conn[1])
		pthread_cond_broadcast(&data->conn[1]->cond); // unlock mutex
	if (data->conn[0])
		data->conn[0] = NULL;
	if (data->conn[1])
		data->conn[1] = NULL;
	data->compiles++;
	pthread_mutex_unlock(&s->mut_array);
}

void	start_debug(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut_prnt);
	printf("%d %d is debugging\n", timer(s), data->id);
	pthread_mutex_unlock(&s->mut_prnt);
}

void	start_refactor(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut_prnt);
	printf("%d %d is refactoring\n", timer(s), data->id);
	pthread_mutex_unlock(&s->mut_prnt);
}

bool	stages(t_coder *data)
{
	t_span			*s;
	t_dongle		*adj_dongles[2];

	s = (t_span *) data->s;
	adj_dongles[0] = data->conn[0];
	adj_dongles[1] = data->conn[1];
	start_comp(s, data);
	if (!wait_check(s, s->t_compile))
		return (false);
	finish_comp(s, data);
	start_debug(s, data);
	if (!wait_check(s, s->t_debug))
		return (false);
	start_refactor(s, data);
	if (!wait_check(s, s->t_refactor))
		return (false);
	pthread_mutex_lock(&s->mut_array);
	pthread_mutex_lock(&s->mut_time);
	gettimeofday(&data->req_t, NULL);
	pthread_mutex_unlock(&s->mut_time);
	enq_heapq(&adj_dongles[0]->h, (void *) data, fifo);
	enq_heapq(&adj_dongles[1]->h, (void *) data, fifo);
	data->is_active = true;
	s->to_schedule = true;
	pthread_mutex_unlock(&s->mut_array);
	return (true);
}
