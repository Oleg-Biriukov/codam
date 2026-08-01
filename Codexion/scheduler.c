/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:12:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/01 16:39:54 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	fifo(t_array *a1, t_array *a2)
{
	struct timeval	d1_t;
	struct timeval	d2_t;
	struct timeval	start;

	start = ((t_span *)((t_coder *) a1->data)->s)->start;
	d1_t = ((t_coder *) a1->data)->req_t;
	d2_t = ((t_coder *) a2->data)->req_t;
	if (interval(start, d1_t) < interval(start, d2_t))
		return (1);
	return (0);
}

static int	edf(t_array *a1, t_array *a2)
{
	t_coder			*data1;
	t_coder			*data2;
	unsigned int	t_left1;
	unsigned int	t_left2;

	data1 = (t_coder *) a1->data;
	data2 = (t_coder *) a2->data;
	t_left1 = interval(data1->b_interv_s, data1->b_interv_e) / 1000;
	t_left2 = interval(data2->b_interv_s, data2->b_interv_e) / 1000;
	// printf("%d and %d\n", t_left1, t_left2);
	if (t_left1 < t_left2)
		return (1);
	return (0);
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
	}
	pthread_mutex_unlock(&s->mut);
}

static void	scheduling(t_span *s, int (_by)(t_array *, t_array *))
{
	unsigned int	i;
	unsigned int	len_c;
	t_array			*a;
	struct timeval	now;
	struct timespec	wait;

	while (1)
	{
		i = 0;
		pthread_mutex_lock(&s->mut);
		while (s->to_schedule != true)
		{
			gettimeofday(&now, NULL);
			wait = convert(now, 300);
			if (pthread_cond_timedwait(&s->c_to_schedule, &s->mut, &wait) == ETIMEDOUT)
			{
				if (s->is_failed || s->is_over || s->is_burnout)
					return ((void) pthread_mutex_unlock(&s->mut));
				continue ;
			}
			break ;
		}
		la_sort(s->coders, _by);
		// t_array *ara = s->coders;
		// t_coder *d;
		// while(ara)
		// {
		// 	d = (t_coder *) ara->data;
		// 	printf("%d - > %lu ", d->id, interval(d->b_interv_s, d->b_interv_e));
		// 	ara=ara->next;
		// }
		// printf("\n");
		s->to_schedule = false;
		len_c = s->n_coders;
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
