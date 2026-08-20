/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stages.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:05:07 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/20 04:10:09 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	start_comp(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut_prnt);
	printf("%d %d is compiling\n", timer(s), data->id);
	pthread_mutex_unlock(&s->mut_prnt);
	pthread_mutex_lock(&s->mut_array);
	data->is_active = false;
	pthread_mutex_unlock(&s->mut_array);
}

static void	finish_comp(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut_array);
	if (data->conn[0])
	{
		pthread_mutex_lock(&s->mut_time);
		gettimeofday(&data->conn[0]->s_cooldown, NULL);
		pthread_mutex_unlock(&s->mut_time);
		data->conn[0]->is_active = true;
		data->conn[0] = NULL;
	}
	if (data->conn[1])
	{
		pthread_mutex_lock(&s->mut_time);
		gettimeofday(&data->conn[1]->s_cooldown, NULL);
		pthread_mutex_unlock(&s->mut_time);
		data->conn[1]->is_active = true;
		data->conn[1] = NULL;
	}
	data->compiles++;
	pthread_mutex_unlock(&s->mut_array);
}

static void	start_debug(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut_prnt);
	printf("%d %d is debugging\n", timer(s), data->id);
	pthread_mutex_unlock(&s->mut_prnt);
}

static void	start_refactor(t_span *s, t_coder *data)
{
	pthread_mutex_lock(&s->mut_prnt);
	printf("%d %d is refactoring\n", timer(s), data->id);
	pthread_mutex_unlock(&s->mut_prnt);
}

bool	stages(t_coder *data)
{
	t_span			*s;
	t_dongle		*adj_dngl[2];

	s = (t_span *) data->s;
	adj_dngl[0] = data->conn[0];
	adj_dngl[1] = data->conn[1];
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
	pthread_mutex_lock(&s->mut_time);
	gettimeofday(&data->req_t, NULL);
	pthread_mutex_unlock(&s->mut_time);
	requests_for_dongles(s, data, adj_dngl);
	pthread_mutex_lock(&s->mut_array);
	pthread_cond_broadcast(&s->c_to_schedule);
	data->is_active = true;
	s->to_schedule = true;
	return (pthread_mutex_unlock(&s->mut_array), true);
}
