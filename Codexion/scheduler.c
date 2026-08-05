/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:12:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/05 18:08:11 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	fifo(t_array *a1, t_array *a2)
{
	struct timeval	d1_t;
	struct timeval	d2_t;
	struct timeval	start;
	t_span			*s;

	start = ((t_span *)((t_coder *) a1->data)->s)->start;
	d1_t = ((t_coder *) a1->data)->req_t;
	d2_t = ((t_coder *) a2->data)->req_t;
	s = (t_span *) ((t_coder *) a2->data)->s; 
	pthread_mutex_lock(&s->mut_time);
	if (d1_t.tv_sec < d2_t.tv_sec)
		return (pthread_mutex_unlock(&s->mut_time), 1);
	else if (d1_t.tv_sec > d2_t.tv_sec)
		return (pthread_mutex_unlock(&s->mut_time), 0);
	else if (d1_t.tv_sec == d2_t.tv_sec)
	{
		if (d1_t.tv_usec < d2_t.tv_usec)
			return (pthread_mutex_unlock(&s->mut_time), 1);
		else if (d1_t.tv_usec > d2_t.tv_usec)
			return (pthread_mutex_unlock(&s->mut_time), 0);
	}
	return (pthread_mutex_unlock(&s->mut_time), 1);
}
static int	edf(t_array *a1, t_array *a2)
{
    t_coder			*data1;
    t_coder			*data2;
    __int64_t		deadline1;
    __int64_t		deadline2;
    __int64_t		wait1;
    __int64_t		wait2;
    struct timeval	now;
    t_span			*s;
	bool			by_comp;

    data1 = (t_coder *) a1->data;
    data2 = (t_coder *) a2->data;
    s = (t_span *) data1->s;
    gettimeofday(&now, NULL);
	pthread_mutex_lock(&s->mut_array);
	by_comp = data1->compiles < data2->compiles;
	pthread_mutex_unlock(&s->mut_array);
	pthread_mutex_lock(&s->mut_time);
	wait1 = interval(data1->b_interv_s, data1->b_interv_e) / 1000;
    wait2 = interval(data2->b_interv_s, data2->b_interv_e) / 1000;
    deadline1 = (data1->b_interv_s.tv_sec * 1000000LL + data1->b_interv_s.tv_usec)
        + wait1;
    deadline2 = (data2->b_interv_s.tv_sec * 1000000LL + data2->b_interv_s.tv_usec)
        + wait2;
	pthread_mutex_unlock(&s->mut_time);
    if (wait1 > (s->t_burnout / 2) && wait2 > (s->t_burnout / 2))
    {
        if (wait1 > wait2)
            return (1);
        if (wait1 < wait2)
            return (0);
    }
    else if (wait1 > (s->t_burnout / 2))
        return (1);
    else if (wait2 > (s->t_burnout / 2))
        return (0);
    if (deadline1 < deadline2)
        return (1);
    if (deadline1 > deadline2)
        return (0);
    return (by_comp);
}

static bool	is_right_coder(t_span *s, t_array *a)
{
	t_dongle		*ldata;
	t_dongle		*rdata;
	t_coder			*cdata;
	bool			assigned;

	pthread_mutex_lock(&s->mut_array);
	assigned = false;
	cdata = (t_coder *) a->data;
	rdata = (t_dongle *)(a->prev)->data;
	ldata = (t_dongle *)(a->next)->data;
	if (rdata->is_active
		&& ldata->is_active
		&& !cdata->is_done
		&& cdata->is_active
		&& ldata != rdata)
	{
		assigned = true;
		cdata->conn[0] = rdata;
		cdata->conn[1] = ldata;
		rdata->is_active = false;
		ldata->is_active = false;
		pthread_mutex_lock(&s->mut_time);
		gettimeofday(&cdata->b_interv_s, NULL);
		pthread_mutex_unlock(&s->mut_time);
		pthread_cond_broadcast(&cdata->cond);
	}
	pthread_mutex_unlock(&s->mut_array);
	return (assigned);
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
		pthread_mutex_lock(&s->mut_array);
		while (s->to_schedule != true)
		{
			gettimeofday(&now, NULL);
			wait = convert(now, 300);
			if (pthread_cond_timedwait(&s->c_to_schedule, &s->mut_array, &wait) == ETIMEDOUT)
			{
				pthread_mutex_lock(&s->mut);
				if (s->is_failed || s->is_over || s->is_burnout)
					return (pthread_mutex_unlock(&s->mut), (void) pthread_mutex_unlock(&s->mut_array));
				pthread_mutex_unlock(&s->mut);
				continue ;
			}
			break ;
		}
		s->to_schedule = false;
		len_c = s->n_coders;
		pthread_mutex_unlock(&s->mut_array);
		la_sort(s->coders, _by, s);
		while (i < len_c)
		{
			a = find_elem(s->workspace, get_elem(s->coders, i++));
			pthread_mutex_lock(&s->mut);
			if (s->is_over || s->is_failed || s->is_burnout)
				return ((void) pthread_mutex_unlock(&s->mut));
			pthread_mutex_unlock(&s->mut);
			if (is_right_coder(s, a))
				break ;
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
