/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 10:16:37 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/08 16:52:33 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	awaiting_for_connection(t_dongle *data)
{
	t_span			*s;
	struct timespec	wait;
	struct timeval	now;

	s = (t_span *) data->s;
	pthread_mutex_lock(&s->mut_array);
	while (data->is_cooldown == false)
	{
		gettimeofday(&now, NULL);
		wait = convert(now, 100);
		if (pthread_cond_timedwait(&data->cond, &s->mut_array, &wait) == ETIMEDOUT)
		{
			pthread_mutex_lock(&s->mut);
			if (s->is_failed || s->is_over || s->is_burnout)
				return (pthread_mutex_unlock(&s->mut), 
					pthread_mutex_unlock(&s->mut_array), false);
			pthread_mutex_unlock(&s->mut);
			continue ;
		}
		break ;
	}
	pthread_mutex_unlock(&s->mut_array);
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
		pthread_mutex_lock(&s->mut_array);
		data->is_cooldown = false;
		data->is_active = true;
		s->to_schedule = true;
		pthread_cond_broadcast(&s->c_to_schedule);
		pthread_mutex_unlock(&s->mut_array);
		if (RUNNING_ON_VALGRIND)
			usleep(30);
	}
}

bool	init_dongle(t_span *s)
{
	t_array		*array;
	t_dongle	*data;

	array = NULL;
	pthread_mutex_lock(&s->mut_array);
	pthread_mutex_unlock(&s->mut_array);
	while (la_len(la_start(array)) != s->n_coders)
	{
		data = malloc(sizeof(t_dongle));
		array = la_append(array, data);
		if (!data || !array)
			return (false);
		
		pthread_cond_init(&data->cond, NULL);
		data->id = la_len(la_start(array));
		data->s = (void *) s;
		data->h = (t_heapq) {{}, 0};
		data->is_cooldown = false;
		data->is_active = true;
	}
	s->dongle = la_start(array);
	return (true);
}
