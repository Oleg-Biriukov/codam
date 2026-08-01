/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:12:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/01 13:22:25 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	fifo(t_array *a1, t_array *a2)
{
	struct timeval	d1_t;
	struct timeval	d2_t;

	d1_t = ((t_coder *) a1->data)->req_t;
	d2_t = ((t_coder *) a2->data)->req_t;
	if (d1_t.tv_sec * 1000000L + d1_t.tv_usec
		< d2_t.tv_sec * 1000000L + d2_t.tv_usec)
		return (1);
	return (-1);
}

static int	edf(t_array *a1, t_array *a2)
{
	t_coder			*data1;
	t_coder			*data2;
	unsigned int	t_left1;
	unsigned int	t_left2;

	data1 = (t_coder *) a1->data;
	data2 = (t_coder *) a2->data;
	t_left1 = interval(data1->b_interv_s, data1->b_interv_e);
	t_left2 = interval(data2->b_interv_s, data2->b_interv_e);
	if (t_left1 < t_left2)
		return (1);
	return (-1);
}

static void	is_right_coder(t_span *s, t_array *a)
{
	t_dongle		*ldata;
	t_dongle		*rdata;
	t_coder			*cdata;

	pthread_mutex_lock(&s->mut);
	cdata = (t_coder *) a->data;
	rdata = (t_dongle *)(a->prev)->data;
	ldata = (t_dongle *)(a->next)->data;
	if (rdata->is_active
		&& ldata->is_active
		&& !cdata->is_done
		&& cdata->is_active
		&& ldata != rdata)
	{
		cdata->conn[0] = rdata;
		cdata->conn[1] = ldata;
		rdata->is_active = false;
		ldata->is_active = false;
		pthread_cond_broadcast(&cdata->cond);
		if (s->circle == 0)
			s->circle = s->n_coders;
		else
			s->circle--;
	}
	pthread_mutex_unlock(&s->mut);
}

static void	scheduling(t_span *s, int (_by)(t_array *, t_array *))
{
	unsigned int	i;
	unsigned int	len_c;
	t_array			*a;

	while (1)
	{
		i = 0;
		pthread_mutex_lock(&s->mut);
		if (s->circle == 0)
			la_sort(s->coders, _by);
		len_c = la_len(s->coders);
		pthread_mutex_unlock(&s->mut);
		while (i < len_c)
		{
			pthread_mutex_lock(&s->mut);
			a = find_elem(s->workspace, get_elem(s->coders, i++));
			if (s->is_over || s->is_failed || s->is_burnout)
				return ((void) pthread_mutex_unlock(&s->mut));
			pthread_mutex_unlock(&s->mut);
			is_right_coder(s, a);
			if (RUNNING_ON_VALGRIND)
				usleep(30);
		}
	}
}

bool	scheduler(t_span *s)
{
	if (!strcmp(s->schdlr, "fifo"))
		return (scheduling(s, fifo), false);
	else
		return (scheduling(s, edf), false);
}
