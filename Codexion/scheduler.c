/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:12:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/09 18:20:10 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	fifo(void	*data1, void	*data2)
{
	struct timeval	d1_t;
	struct timeval	d2_t;
	t_span			*s;

	d1_t = ((t_coder *) data1)->req_t;
	d2_t = ((t_coder *) data2)->req_t;
	s = (t_span *) ((t_coder *) data1)->s; 
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
	return (pthread_mutex_unlock(&s->mut_time),
		((t_coder *) data1)->id < ((t_coder *) data2)->id);
}

int	edf(void *data1, void *data2)
{
    __int64_t		deadline1;
    __int64_t		deadline2;
    __int64_t		wait1;
    __int64_t		wait2;
    struct timeval	now;
    t_span			*s;
	t_coder			*d1;
	t_coder			*d2;

	d1 = (t_coder *) data1;
	d2 = (t_coder *) data2;
	s = (t_span *) d1->s;
    gettimeofday(&now, NULL);
	pthread_mutex_lock(&s->mut_time);
	wait1 = interval(d1->b_interv_s, d1->b_interv_e) / 1000;
    wait2 = interval(d2->b_interv_s, d2->b_interv_e) / 1000;
    deadline1 = (d1->b_interv_s.tv_sec * 1000000LL + d1->b_interv_s.tv_usec)
        + wait1;
    deadline2 = (d2->b_interv_s.tv_sec * 1000000LL + d2->b_interv_s.tv_usec)
        + wait2;
	pthread_mutex_unlock(&s->mut_time);
    if (wait1 > (s->t_burnout / 2) && wait2 > (s->t_burnout / 2))
    {
        if (wait1 > wait2)
            return (0);
        if (wait1 < wait2)
            return (1);
    }
    else if (wait1 > (s->t_burnout / 2))
        return (0);
    else if (wait2 > (s->t_burnout / 2))
        return (1);
    if (deadline1 < deadline2)
        return (0);
    if (deadline1 > deadline2)
        return (1);
    return (fifo(data1, data2));
}

static void take_left_dongle(t_span *s, t_coder	*cdata, t_dongle *ldata)
{
	if (ldata->is_active)
	{
		pthread_mutex_lock(&s->mut_prnt);
		printf("%d %d has taken a dongle%d\n", timer(s), cdata->id, ldata->id);
		pthread_mutex_unlock(&s->mut_prnt);
		cdata->conn[0] = ldata;
		ldata->is_active = false;
	}
}

static void take_right_dongle(t_span *s, t_coder	*cdata, t_dongle *rdata)
{
	if (rdata->is_active)
	{
		pthread_mutex_lock(&s->mut_prnt);
		printf("%d %d has taken a dongle%d\n", timer(s), cdata->id, rdata->id);
		pthread_mutex_unlock(&s->mut_prnt);
		cdata->conn[1] = rdata;
		rdata->is_active = false;
	}
}

static bool	is_right_coder(t_span *s, t_array *c, t_array *d) // ?
{
	t_dongle		*ldata;
	t_dongle		*rdata;
	t_coder			*cdata;
	bool			assigned;

	assigned = false;
	cdata = (t_coder *) c->data;
	rdata = (t_dongle *)(c->prev)->data;
	ldata = (t_dongle *)(c->next)->data;
	if (!cdata->is_done
		&& cdata->is_active
		&& ldata != rdata)
	{
		assigned = true;
		if (d == c->prev && !cdata->conn[1])
    		take_right_dongle(s, cdata, rdata);
		if (d == c->next && !cdata->conn[0])
    		take_left_dongle(s, cdata, ldata);
		// pthread_mutex_lock(&s->mut_time);
		// gettimeofday(&cdata->b_interv_s, NULL);
		// pthread_mutex_unlock(&s->mut_time);
		if (cdata->conn[1] && cdata->conn[0])
			pthread_cond_broadcast(&cdata->cond);
	}
	return (assigned);
}

static void	scheduling(t_span *s, int (_by)(void *, void *))
{
	// unsigned int	i;
	// unsigned int	len_c;
	t_array			*a;
	t_dongle		*data;
	void			*coder;
	// struct timeval	now;
	// struct timespec	wait;

	// len_c = s->n_coders;
	pthread_mutex_lock(&s->mut_array);
	pthread_mutex_unlock(&s->mut_array);
	while (1)
	{
		// i = 0;
		// pthread_mutex_lock(&s->mut_array);
		// while (s->to_schedule != true)
		// {
		// 	gettimeofday(&now, NULL);
		// 	wait = convert(now, 100);
		// 	if (pthread_cond_timedwait(&s->c_to_schedule, &s->mut_array, &wait) == ETIMEDOUT)
		// 	{
		// 		pthread_mutex_lock(&s->mut);
		// 		if (s->is_failed || s->is_over || s->is_burnout)
		// 			return (pthread_mutex_unlock(&s->mut), (void) pthread_mutex_unlock(&s->mut_array));
		// 		pthread_mutex_unlock(&s->mut);
		// 		continue ;
		// 	}
		// 	break ;
		// }
		// s->to_schedule = false;
		// pthread_mutex_unlock(&s->mut_array);
		a = s->workspace->next;
		while (a)
		{
			pthread_mutex_lock(&s->mut);
			if (s->is_over || s->is_failed || s->is_burnout)
				return ((void) pthread_mutex_unlock(&s->mut));
			pthread_mutex_unlock(&s->mut);
			data = (t_dongle *) a->data;
			pthread_mutex_lock(&s->mut_array);
			coder = deq_heapq(&data->h, _by);
			pthread_mutex_unlock(&s->mut_array);
			if (coder == a->next->data)
				coder = a->next;
			else
				coder = a->prev;
			pthread_mutex_lock(&s->mut_array);
			if (data->is_active)
				is_right_coder(s, coder, a);
			pthread_mutex_unlock(&s->mut_array);
			a = a->next->next;
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


// la_sort(s->coders, _by, s);
// 		while (i < len_c)
// 		{
// 			a = find_elem(s->workspace, get_elem(s->coders, i++));
// 			pthread_mutex_lock(&s->mut);
// 			if (s->is_over || s->is_failed || s->is_burnout)
// 				return ((void) pthread_mutex_unlock(&s->mut));
// 			pthread_mutex_unlock(&s->mut);
// 			is_right_coder(s, a);
// 			if (RUNNING_ON_VALGRIND)
// 				usleep(30);
// 		}