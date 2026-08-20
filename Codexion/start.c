/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 12:33:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/20 03:36:58 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	create_coders(t_span *s, t_array *a)
{
	t_coder		*c_data;
	t_dongle	*ld_data;
	t_dongle	*rd_data;

	s->is_begging = true;
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
		scheduling(s, a->next);
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
		usleep(1);
	}
}

static void	finish(t_array *a)
{
	t_coder			*c_data;

	while (a)
	{
		c_data = (t_coder *) a->data;
		pthread_join(c_data->t, NULL);
		a = a->next;
	}
}

bool	start(t_span *s)
{
	unsigned int	total_c;
	unsigned int	n_coders;
	pthread_t		t;

	if (!strcmp(s->schdlr, FIFO))
		s->_by = fifo;
	else
		s->_by = edf;
	pthread_mutex_lock(&s->mut_array);
	gettimeofday(&s->start, NULL);
	create_coders(s, s->coders);
	total_c = s->n_coders * s->n_compiles;
	n_coders = s->n_coders;
	if (pthread_create(&t, NULL, (void *) &monitor, s) != 0)
		fail(s);
	pthread_mutex_unlock(&s->mut_array);
	if (!s->is_failed)
		awaiting(s, s->workspace, total_c);
	pthread_mutex_lock(&s->mut);
	s->is_over = true;
	pthread_mutex_unlock(&s->mut);
	finish(s->coders);
	pthread_join(t, NULL);
	return (!s->is_failed);
}
