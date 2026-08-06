/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/06 17:46:17 by obirukov         ###   ########.fr       */
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
			|| s->is_burnout)
			return ((void) pthread_mutex_unlock(&s->mut));
		pthread_mutex_unlock(&s->mut);
		pthread_mutex_lock(&s->mut_array);
		if (data->is_done)
			return ((void) pthread_mutex_unlock(&s->mut_array));
		pthread_mutex_unlock(&s->mut_array);
		pthread_mutex_lock(&s->mut_time);
		gettimeofday(&data->b_interv_e, NULL);
		if (interval(data->b_interv_s, data->b_interv_e) > s->t_burnout * 1000)
		{
			pthread_mutex_unlock(&s->mut_time);
			break ;
		}
		pthread_mutex_unlock(&s->mut_time);
		if (RUNNING_ON_VALGRIND)
			usleep(30);
	}
	pthread_mutex_lock(&s->mut);
	s->is_burnout = true;
	pthread_mutex_unlock(&s->mut);
	pthread_mutex_lock(&s->mut_prnt);
	printf("%d %d burned out\n", timer(s), data->id);
	pthread_mutex_unlock(&s->mut_prnt);
}

static bool	awaiting_for_connection(t_coder	*data)
{
	t_span			*s;
	struct timespec	wait;
	struct timeval	now;

	s = (t_span *) data->s;
	pthread_mutex_lock(&s->mut_array);
	while (!(data->conn[0] && data->conn[1]))
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
	return (pthread_mutex_unlock(&s->mut_array), true);
}

void	coder(t_coder *data)
{
	unsigned int	n_comp;
	unsigned int	c_comp;
	t_span			*s;

	s = (t_span *) data->s;
	pthread_mutex_lock(&s->mut_array);
	n_comp = s->n_compiles;
	c_comp = data->compiles;
	pthread_mutex_unlock(&s->mut_array);
	while (c_comp != n_comp)
	{
		pthread_mutex_lock(&s->mut_array);
		c_comp = data->compiles;
		pthread_mutex_unlock(&s->mut_array);
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
	pthread_mutex_lock(&s->mut_array);
	data->is_done = true;
	pthread_mutex_unlock(&s->mut_array);
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
		gettimeofday(&data->b_interv_s, NULL);
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
