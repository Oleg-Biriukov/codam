/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stages.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:05:07 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/21 19:18:45 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int		stages(t_coder *data)
{
	t_span			*s;

	s = (t_span *) data->s;

	pthread_mutex_lock(&s->mut);
	data->is_active = 0;
	printf("[%d ms] START_COMPILE\tC%d\n", s->time , data->id);
	gettimeofday(&data->b_interv_s, NULL);
	pthread_mutex_unlock(&s->mut);

	if (wait_check(s, s->t_compile) != 0)
		return (-1);

	pthread_mutex_lock(&s->mut);
	printf("[%d ms] RELEASE_DONGLE C%d D%d\n", s->time, data->id, data->conn[1]->id);
	printf("[%d ms] RELEASE_DONGLE C%d D%d\n", s->time, data->id, data->conn[0]->id);
	data->conn[0]->is_cooldown = 1;
	data->conn[1]->is_cooldown = 1;
	pthread_cond_broadcast(&data->conn[0]->cond);
	pthread_cond_broadcast(&data->conn[1]->cond);
	data->conn[0] = NULL;
	data->conn[1] = NULL;
	data->compiles++;
	pthread_mutex_unlock(&s->mut);

	pthread_mutex_lock(&s->mut);
	printf("[%d ms] START_DEBUG\tC%d\n", s->time , data->id);
	pthread_mutex_unlock(&s->mut);

	if (wait_check(s, s->t_debug) != 0)
		return (-1);
	
	pthread_mutex_lock(&s->mut);
	printf("[%d ms] START_REFACTOR\tC%d\n", s->time , data->id);
	pthread_mutex_unlock(&s->mut);

	if (wait_check(s, s->t_refactor) != 0)
		return (-1);
	
	pthread_mutex_lock(&s->mut);
	data->is_active = 1;
	pthread_mutex_unlock(&s->mut);
	return (0);
}
