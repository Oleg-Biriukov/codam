/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 10:16:37 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/20 04:16:48 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	requests_for_dongles(t_span *s, t_coder *data, t_dongle *adj_dngl[])
{
	pthread_mutex_lock(&s->mut_array);
	enq_heapq(&adj_dngl[0]->h, (void *) data, s->_by);
	pthread_mutex_unlock(&s->mut_array);
	pthread_mutex_lock(&s->mut_array);
	enq_heapq(&adj_dngl[1]->h, (void *) data, s->_by);
	pthread_mutex_unlock(&s->mut_array);
}

bool	is_cooldown(t_dongle *data)
{
	struct timeval	now;
	t_span			*s;

	s = (t_span *) data->s;
	gettimeofday(&now, NULL);
	pthread_mutex_lock(&s->mut_time);
	if (data->s_cooldown.tv_sec == 0 && data->s_cooldown.tv_usec == 0)
		return (pthread_mutex_unlock(&s->mut_time), false);
	if (interval(data->s_cooldown, now) > s->d_cooldown * 1000)
		return (pthread_mutex_unlock(&s->mut_time), false);
	pthread_mutex_unlock(&s->mut_time);
	return (true);
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
		data->id = la_len(la_start(array));
		data->s = (void *) s;
		data->h = (t_heapq){{}, 0};
		data->is_active = true;
		data->s_cooldown.tv_sec = 0;
		data->s_cooldown.tv_usec = 0;
	}
	s->dongle = la_start(array);
	return (true);
}
