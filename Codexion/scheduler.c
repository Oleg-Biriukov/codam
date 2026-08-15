/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:12:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/15 14:13:43 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	fifo(void	*data1, void	*data2)
{
	struct timeval	d1_t;
	struct timeval	d2_t;
	t_span			*s;

	s = (t_span *) ((t_coder *) data1)->s; 
	pthread_mutex_lock(&s->mut_time);
	d1_t = ((t_coder *) data1)->req_t;
	d2_t = ((t_coder *) data2)->req_t;
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
    struct timeval		deadline1;
    struct timeval		deadline2;
    t_span				*s;
	t_coder				*d1;
	t_coder				*d2;

	d1 = (t_coder *) data1;
	d2 = (t_coder *) data2;
	s = (t_span *) d1->s;
	pthread_mutex_lock(&s->mut_time);
    deadline1.tv_sec = d1->b_interv_s.tv_sec + (s->t_burnout / 1000);
	deadline1.tv_usec = d1->b_interv_s.tv_sec + ((s->t_burnout % 1000) * 1000);
    deadline2.tv_sec = d2->b_interv_s.tv_sec + (s->t_burnout / 1000);
	deadline2.tv_usec = d2->b_interv_s.tv_sec + ((s->t_burnout % 1000) * 1000);
	pthread_mutex_unlock(&s->mut_time);
	if (deadline1.tv_sec < deadline2.tv_sec)
		return (1);
	else if (deadline1.tv_sec > deadline2.tv_sec)
		return (0);
	else if (deadline1.tv_sec == deadline2.tv_sec)
	{
		if (deadline1.tv_usec < deadline2.tv_usec)
			return (1);
		else if (deadline1.tv_usec > deadline2.tv_usec)
			return (0);
	}
    return (fifo(data1, data2));
}

static void take_left_dongle(t_span *s, t_coder	*cdata, t_dongle *ldata)
{
	if (ldata->is_active && !is_cooldown(ldata))
	{
		pthread_mutex_lock(&s->mut_prnt);
		printf("%d %d has taken a dongle\n", timer(s), cdata->id);
		pthread_mutex_unlock(&s->mut_prnt);
		cdata->conn[0] = ldata;
		ldata->is_active = false;
		deq_heapq(&ldata->h, s->_by);
	}
}

static void take_right_dongle(t_span *s, t_coder	*cdata, t_dongle *rdata)
{
	if (rdata->is_active && !is_cooldown(rdata))
	{
		pthread_mutex_lock(&s->mut_prnt);
		printf("%d %d has taken a dongle\n", timer(s), cdata->id);
		pthread_mutex_unlock(&s->mut_prnt);
		cdata->conn[1] = rdata;
		rdata->is_active = false;
		deq_heapq(&rdata->h, s->_by);
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
		if (d == c->prev && !cdata->conn[1])
    		take_right_dongle(s, cdata, rdata);
		if (d == c->next && !cdata->conn[0])
    		take_left_dongle(s, cdata, ldata);
		if (cdata->conn[1] && cdata->conn[0])
		{
			pthread_mutex_lock(&s->mut_time);
			gettimeofday(&cdata->b_interv_s, NULL);
			pthread_mutex_unlock(&s->mut_time);
			return(pthread_cond_broadcast(&cdata->cond), true);
		}
	}
	return (assigned);
}


void	scheduling(t_span *s)
{
	t_array			*a;
	t_dongle		*data;
	void			*coder;
	// struct timeval	now;
	// struct timespec	wait;

	pthread_mutex_lock(&s->mut_array);
	pthread_mutex_unlock(&s->mut_array);
	while (1)
	{
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
			if (!is_cooldown(data) && data->is_active)
				coder = peek(&data->h);
			else
				coder = NULL;
			pthread_mutex_unlock(&s->mut_array);
			if (coder == a->next->data)
				coder = a->next;
			if (coder == a->prev->data)
				coder = a->prev;
			pthread_mutex_lock(&s->mut_array);
			if (coder)
				is_right_coder(s, coder, a);
			pthread_mutex_unlock(&s->mut_array);
			a = a->next->next;
			if (RUNNING_ON_VALGRIND)
				usleep(30);
			if (a->data == s->dongle->data)
				break ;
		}
	}
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