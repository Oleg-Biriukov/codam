/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stages.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:05:07 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/26 15:40:11 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	start_comp(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut);
	data->is_active = false;
	printf("[%d ms] START_COMPILE\tC%d\n", s->time, data->id);
	gettimeofday(&data->b_interv_s, NULL);
	pthread_mutex_unlock(&s->mut);
}

void	finish_comp(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut);
	printf("[%d ms] RELEASE_DONGLE C%d D%d\n",
		s->time, data->id, data->conn[1]->id);
	printf("[%d ms] RELEASE_DONGLE C%d D%d\n",
		s->time, data->id, data->conn[0]->id);
	data->conn[0]->is_cooldown = true;
	data->conn[1]->is_cooldown = true;
	pthread_cond_broadcast(&data->conn[0]->cond);
	pthread_cond_broadcast(&data->conn[1]->cond);
	data->conn[0] = NULL;
	data->conn[1] = NULL;
	data->compiles++;
	pthread_mutex_unlock(&s->mut);
}

void	start_debug(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut);
	printf("[%d ms] START_DEBUG\tC%d\n", s->time, data->id);
	pthread_mutex_unlock(&s->mut);
}

void	start_refactor(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut);
	printf("[%d ms] START_REFACTOR\tC%d\n", s->time, data->id);
	pthread_mutex_unlock(&s->mut);
}

bool	stages(t_coder *data)
{
	t_span			*s;

	s = (t_span *) data->s;
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
	pthread_mutex_lock(&s->mut);
	data->is_active = true;
	pthread_mutex_unlock(&s->mut);
	return (true);
}
