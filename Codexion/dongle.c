/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 10:16:37 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/26 16:22:12 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	awaiting_for_connection(t_dongle *data)
{
	t_span			*s;
	struct timespec	wait;
	struct timeval	now;

	s = (t_span *) data->s;
	pthread_mutex_lock(&s->mut);
	while (data->is_cooldown == false)
	{
		gettimeofday(&now, NULL);
		wait = convert(now, 1000);
		if (pthread_cond_timedwait(&data->cond, &s->mut, &wait) == ETIMEDOUT)
		{
			if (s->is_failed || s->is_over)
				return (pthread_mutex_unlock(&s->mut), false);
			continue ;
		}
		break ;
	}
	pthread_mutex_unlock(&s->mut);
	return (true);
}

void	dongle(t_dongle *data)
{
	t_span	*s;

	s = (t_span *) data->s;
	while (1)
	{
		if (!awaiting_for_connection(data))
			return ;
		if (!wait_check(s, s->d_cooldown))
			return ;
		pthread_mutex_lock(&s->mut);
		data->is_cooldown = false;
		data->is_active = true;
		pthread_mutex_unlock(&s->mut);
		if (RUNNING_ON_VALGRIND)
		{
			usleep(30);
		}
	}
}

bool	init_dongle(t_span *s)
{
	t_array		*array;
	t_dongle	*data;

	array = NULL;
	while (la_len(la_start(array)) != s->n_coders)
	{
		data = malloc(sizeof(t_dongle));
		array = la_append(array, data);
		if (!data || !array)
			return (false);
		gettimeofday(&data->start, NULL);
		pthread_cond_init(&data->cond, NULL);
		data->id = la_len(la_start(array));
		data->s = (void *) s;
		data->is_cooldown = false;
		data->is_active = true;
	}
	s->dongle = la_start(array);
	return (true);
}
