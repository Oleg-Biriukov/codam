/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stages.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:05:07 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/02 16:05:57 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	start_comp(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut);
	data->is_active = false;
	printf("%d %d is compiling\n", timer(s), data->id);
	gettimeofday(&data->b_interv_s, NULL);
	pthread_mutex_unlock(&s->mut);
}

void	finish_comp(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut);
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
	printf("%d %d is debugging\n", timer(s), data->id);
	pthread_mutex_unlock(&s->mut);
}

void	start_refactor(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut);
	printf("%d %d is refactoring\n", timer(s), data->id);
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
	gettimeofday(&data->req_t, NULL);
	s->to_schedule = true;
	pthread_cond_broadcast(&s->c_to_schedule);
	pthread_mutex_unlock(&s->mut);
	return (true);
}
