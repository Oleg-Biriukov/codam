/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/01 13:40:42 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	detect_b(t_coder *data)
{
	t_span	*s;

	s = (t_span *) data->s;
	while (1)
	{
		pthread_mutex_lock(&s->mut);
		if (s->is_over || s->is_failed
			|| s->n_compiles == data->compiles || s->is_burnout)
			return ((void) pthread_mutex_unlock(&s->mut));
		pthread_mutex_unlock(&s->mut);
		pthread_mutex_lock(&s->mut);
		gettimeofday(&data->b_interv_e, NULL);
		if (interval(data->b_interv_s, data->b_interv_e) > s->t_burnout * 1000)
		{
			pthread_mutex_unlock(&s->mut);
			break ;
		}
		pthread_mutex_unlock(&s->mut);
		if (RUNNING_ON_VALGRIND)
			usleep(30);
	}
	pthread_mutex_lock(&s->mut);
	s->is_burnout = true;
	printf("%d %d burned out\n", timer(s), data->id);
	pthread_mutex_unlock(&s->mut);
}

static bool	awaiting_for_connection(t_coder	*data)
{
	t_span			*s;
	struct timespec	wait;
	struct timeval	now;

	s = (t_span *) data->s;
	pthread_mutex_lock(&s->mut);
	while (!(data->conn[0] && data->conn[1]))
	{
		gettimeofday(&now, NULL);
		wait = convert(now, 300);
		if (pthread_cond_timedwait(&data->cond, &s->mut, &wait) == ETIMEDOUT)
		{
			if (s->is_failed || s->is_over || s->is_burnout)
				return ((void) pthread_mutex_unlock(&s->mut), false);
			continue ;
		}
		break ;
	}
	printf("%d %d has taken a dongle\n", timer(s), data->id);
	printf("%d %d has taken a dongle\n", timer(s), data->id);
	pthread_mutex_unlock(&s->mut);
	return (true);
}

void	coder(t_coder *data)
{
	unsigned int	n_comp;
	t_span			*s;

	s = (t_span *) data->s;
	pthread_mutex_lock(&s->mut);
	n_comp = s->n_compiles;
	pthread_mutex_unlock(&s->mut);
	while (data->compiles != n_comp)
	{
		if (!awaiting_for_connection(data))
			return ;
		if (!stages(data))
			break ;
		pthread_mutex_lock(&s->mut);
		if (s->is_failed || s->is_over || s->is_burnout)
			return ((void) pthread_mutex_unlock(&s->mut));
		pthread_mutex_unlock(&s->mut);
		if (RUNNING_ON_VALGRIND)
			usleep(30);
	}
	pthread_mutex_lock(&s->mut);
	data->is_done = true;
	pthread_mutex_unlock(&s->mut);
}

bool	init_arrays(t_span *s)
{
	t_array		*array;
	t_coder		*data;

	array = NULL;
	while (la_len(la_start(array)) < s->n_coders)
	{
		data = malloc(sizeof(t_coder));
		array = la_append(array, data);
		if (!array || !data)
			return (false);
		data->id = la_len(la_start(array));
		pthread_cond_init(&data->cond, NULL);
		gettimeofday(&data->start, NULL);
		gettimeofday(&data->req_t, NULL);
		data->s = (void *) s;
		data->is_done = false;
		data->is_active = true;
		data->compiles = false;
		data->conn[0] = NULL;
		data->conn[1] = NULL;
	}
	s->coders = la_start(array);
	return (true);
}
