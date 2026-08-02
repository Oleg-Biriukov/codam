/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:12:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/02 17:45:36 by obirukov         ###   ########.fr       */
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
	if (d1_t.tv_sec < d2_t.tv_sec)
		return (1);
	else if (d1_t.tv_sec > d2_t.tv_sec)
		return (0);
	else if (d1_t.tv_sec == d2_t.tv_sec)
	{
		if (d1_t.tv_usec < d2_t.tv_usec)
			return (1);
		else if (d1_t.tv_usec > d2_t.tv_usec)
			return (0);
	}
	return (1);
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

    data1 = (t_coder *) a1->data;
    data2 = (t_coder *) a2->data;
    s = (t_span *) data1->s;
    gettimeofday(&now, NULL);
    deadline1 = (data1->b_interv_s.tv_sec * 1000000LL + data1->b_interv_s.tv_usec)
        + s->t_burnout * 1000LL;
    deadline2 = (data2->b_interv_s.tv_sec * 1000000LL + data2->b_interv_s.tv_usec)
        + s->t_burnout * 1000LL;
    wait1 = interval(data1->req_t, now) / 1000;
    wait2 = interval(data2->req_t, now) / 1000;
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
    if (data1->id < data2->id)
        return (1);
    return (0);
}
// static int	edf(t_array *a1, t_array *a2)
// {
// 	t_span			*s;
// 	t_coder			*data1;
// 	t_coder			*data2;
// 	__int64_t		deadline1;
// 	__int64_t		deadline2;
// 	__int64_t		wait1;
// 	__int64_t		wait2;
// 	struct timeval	now;

// 	data1 = (t_coder *) a1->data;
// 	data2 = (t_coder *) a2->data;
// 	s = (t_span *) data1->s;
// 	gettimeofday(&now, NULL);
// 	deadline1 = (data1->b_interv_s.tv_sec * 1000000LL + data1->b_interv_s.tv_usec)
// 		+ s->t_burnout;
// 	wait1 = interval(data2->req_t, now) / 1000;
// 	deadline2 = (data2->b_interv_s.tv_sec * 1000000LL + data2->b_interv_s.tv_usec)
// 		+ s->t_burnout;
// 	wait2 = interval(data2->req_t, now) / 1000;
// 	if (wait1 > (s->t_burnout / 2) && wait2 > (s->t_burnout / 2))
// 	{
// 		if (wait1 > wait2)
// 			return (1);
// 		if (wait1 < wait2)
// 			return (0);	
// 	}
// 	else if (wait1 > (s->t_burnout / 2))
// 		return (1);
// 	else if (wait2 > (s->t_burnout / 2))
// 		return (0);
// 	if (deadline1 < deadline2)
// 		return (1);
// 	if (deadline1 > deadline2)
// 		return (0);
// 	if (data1->id < data2->id)
// 		return (1);
// 	return (0);
	
// }

static bool	is_right_coder(t_span *s, t_array *a)
{
	t_dongle		*ldata;
	t_dongle		*rdata;
	t_coder			*cdata;
	bool			assigned;

	pthread_mutex_lock(&s->mut);
	if (timer(s) > 1000 && timer(s) < 1100)
		timer(s);
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
		pthread_cond_broadcast(&cdata->cond);
	}
	pthread_mutex_unlock(&s->mut);
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
		printf("%d sorting...\n", timer(s));
		la_sort(s->coders, _by);
		t_array *ara = s->coders;
		t_coder *d;
		while(ara)
		{
			d = (t_coder *) ara->data;
			if (!strcmp(s->schdlr, "fifo"))
			{	
				long days    = d->req_t.tv_sec / 86400;
				long hours   = (d->req_t.tv_sec % 86400) / 3600;
				long minutes = (d->req_t.tv_sec % 3600) / 60;
				long seconds = d->req_t.tv_sec % 60;
				long millis  = d->req_t.tv_usec / 1000;
						
				printf("%d -> %ldd %ldh %ldm %lds %ldms\n",
					d->id,
					days,
					hours,
					minutes,
					seconds,
					millis);
			}
			else 
			{
				printf("%d -> %ldms\n", d->id, interval(d->b_interv_s, d->b_interv_e) / 1000);
			}
			ara=ara->next;
		}
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
