/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:12:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/20 04:02:55 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	take_left_dongle(t_span *s, t_coder	*cdata, t_dongle *ldata)
{
	pthread_mutex_lock(&s->mut_array);
	if (ldata->is_active && !is_cooldown(ldata))
	{
		pthread_mutex_unlock(&s->mut_array);
		pthread_mutex_lock(&s->mut_prnt);
		printf("%d %d has taken a dongle\n", timer(s), cdata->id);
		pthread_mutex_unlock(&s->mut_prnt);
		pthread_mutex_lock(&s->mut_array);
		deq_heapq(&ldata->h, s->_by);
		cdata->conn[0] = ldata;
		ldata->is_active = false;
	}
	pthread_mutex_unlock(&s->mut_array);
}

static void	take_right_dongle(t_span *s, t_coder	*cdata, t_dongle *rdata)
{
	pthread_mutex_lock(&s->mut_array);
	if (rdata->is_active && !is_cooldown(rdata))
	{
		pthread_mutex_unlock(&s->mut_array);
		pthread_mutex_lock(&s->mut_prnt);
		printf("%d %d has taken a dongle\n", timer(s), cdata->id);
		pthread_mutex_unlock(&s->mut_prnt);
		pthread_mutex_lock(&s->mut_array);
		deq_heapq(&rdata->h, s->_by);
		cdata->conn[1] = rdata;
		rdata->is_active = false;
	}
	pthread_mutex_unlock(&s->mut_array);
}

static void	is_right_coder(t_span *s, t_array *c, t_array *d)
{
	t_dongle		*ldata;
	t_dongle		*rdata;
	t_coder			*cdata;

	cdata = (t_coder *) c->data;
	rdata = (t_dongle *)(c->prev)->data;
	ldata = (t_dongle *)(c->next)->data;
	if (!cdata->is_done
		&& cdata->is_active
		&& ldata != rdata)
	{
		pthread_mutex_unlock(&s->mut_array);
		if (d == c->prev && !cdata->conn[1])
			take_right_dongle(s, cdata, rdata);
		if (d == c->next && !cdata->conn[0])
			take_left_dongle(s, cdata, ldata);
		pthread_mutex_lock(&s->mut_array);
		if (cdata->conn[1] && cdata->conn[0])
		{
			pthread_mutex_lock(&s->mut_time);
			gettimeofday(&cdata->b_interv_s, NULL);
			pthread_mutex_unlock(&s->mut_time);
			return ((void) pthread_cond_broadcast(&cdata->cond));
		}
	}
}

static void	*is_true_coder(t_span *s, t_array *a, t_dongle *data)
{
	void	*coder;

	pthread_mutex_lock(&s->mut_array);
	if (!is_cooldown(data) && data->is_active)
		coder = peek(&data->h);
	else
		coder = NULL;
	pthread_mutex_unlock(&s->mut_array);
	if (coder == a->next->data)
		coder = a->next;
	if (coder == a->prev->data)
		coder = a->prev;
	return (coder);
}

void	scheduling(t_span *s, t_array *a)
{
	t_dongle		*data;
	void			*coder;

	while (a)
	{
		pthread_mutex_lock(&s->mut);
		if (s->is_over || s->is_failed || s->is_burnout)
			return ((void) pthread_mutex_unlock(&s->mut));
		pthread_mutex_unlock(&s->mut);
		data = (t_dongle *) a->data;
		coder = is_true_coder(s, a, data);
		pthread_mutex_lock(&s->mut_array);
		if (coder)
			is_right_coder(s, coder, a);
		pthread_mutex_unlock(&s->mut_array);
		a = a->next->next;
		if (a->data == s->dongle->data)
			break ;
	}
	pthread_mutex_lock(&s->mut_array);
	s->is_begging = false;
	pthread_mutex_unlock(&s->mut_array);
}
