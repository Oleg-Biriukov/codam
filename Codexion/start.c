/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 12:33:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/14 15:59:39 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	create_coders(t_span *s, t_array *a)
{
	t_coder		*c_data;
	t_dongle	*ld_data;
	t_dongle	*rd_data;

	while (a)
	{
		c_data = (t_coder *) a->data;
		rd_data = (t_dongle *) find_elem(s->workspace, a)->prev->data;
		ld_data = (t_dongle *) find_elem(s->workspace, a)->next->data;
		if (pthread_create(&c_data->t, NULL, (void *) coder, c_data) != 0)
			return ((void) fail(s));
		gettimeofday(&c_data->b_interv_s, NULL);
		gettimeofday(&c_data->req_t, NULL);
		if (c_data->id % 2 != 0)
			c_data->req_t.tv_sec -= 10;
		enq_heapq(&rd_data->h, a->data, s->_by);
		enq_heapq(&ld_data->h, a->data, s->_by);
		a = a->next;
	}
}

static void	create_dongle(t_span *s, t_array *a)
{
	t_dongle			*d_data;

	while (a)
	{
		d_data = (t_dongle *) a->data;
		if (pthread_create(&d_data->t, NULL, (void *) dongle, d_data) != 0)
			return ((void) fail(s));
		a = a->next;
	}
}

static void	awaiting(t_span *s, t_array *a, unsigned int total_c)
{
	unsigned int	counter;
	t_coder			*c_data;

	counter = 0;
	while (1)
	{
		if (counter == total_c)
			break ;
		if (a == s->workspace)
			counter = 0;
		pthread_mutex_lock(&s->mut);
		if (s->is_failed || s->is_burnout)
			return ((void) pthread_mutex_unlock(&s->mut));
		pthread_mutex_unlock(&s->mut);
		c_data = (t_coder *) a->data;
		pthread_mutex_lock(&s->mut_array);
		if (c_data->compiles >= s->n_compiles)
			counter += s->n_compiles;
		else
			counter += c_data->compiles;
		pthread_mutex_unlock(&s->mut_array);
		a = a->next->next;
		if (RUNNING_ON_VALGRIND)
			usleep(30);
	}
}

static void	finish(t_array *a, unsigned int n_coders)
{
	t_coder			*c_data;
	t_dongle		*d_data;
	unsigned int	counter;

	counter = 0;
	while (counter != n_coders * 2)
	{
		counter++;
		if (counter % 2 == 0)
		{
			d_data = (t_dongle *) a->data;
			pthread_join(d_data->t, NULL);
		}
		else
		{
			c_data = (t_coder *) a->data;
			pthread_join(c_data->t, NULL);
			// pthread_join(c_data->t_burnout, NULL);
		}
		a = a->next;
	}
}

bool	start(t_span *s)
{
	unsigned int	total_c;
	unsigned int	n_coders;
	t_array			*workspace;
	pthread_t		t[2];

	workspace = s->workspace;
	pthread_mutex_lock(&s->mut_array);
	create_coders(s, s->coders);
	create_dongle(s, s->dongle);
	total_c = s->n_coders * s->n_compiles;
	n_coders = s->n_coders;
	if (pthread_create(&t[0], NULL, (void *) &scheduling, s) != 0)
		fail(s);
	if (pthread_create(&t[1], NULL, (void *) &monitor, s) != 0)
		return (fail(s));
	gettimeofday(&s->start, NULL);
	pthread_mutex_unlock(&s->mut_array);
	if (!s->is_failed)
		awaiting(s, workspace, total_c);
	pthread_mutex_lock(&s->mut);
	s->is_over = true;
	pthread_mutex_unlock(&s->mut);
	finish(workspace, n_coders);
	pthread_join(t[0], NULL);
	pthread_join(t[1], NULL);
	return (!s->is_failed);
}
