/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 03:27:05 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/20 04:06:38 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	who_is_earlier(struct timeval d1_t, struct timeval d2_t,
															void *d1, void *d2)
{
	if (d1_t.tv_sec < d2_t.tv_sec)
		return (true);
	else if (d1_t.tv_sec > d2_t.tv_sec)
		return (false);
	else if (d1_t.tv_sec == d2_t.tv_sec)
	{
		if (d1_t.tv_usec < d2_t.tv_usec)
			return (true);
		else if (d1_t.tv_usec > d2_t.tv_usec)
			return (false);
		else
			return (((t_coder *) d1)->id < ((t_coder *) d2)->id);
	}
	return (false);
}

bool	fifo(void	*data1, void	*data2)
{
	struct timeval	d1_t;
	struct timeval	d2_t;
	t_span			*s;

	s = (t_span *)((t_coder *) data1)->s;
	pthread_mutex_unlock(&s->mut_array);
	pthread_mutex_lock(&s->mut_time);
	d1_t = ((t_coder *) data1)->req_t;
	d2_t = ((t_coder *) data2)->req_t;
	pthread_mutex_unlock(&s->mut_time);
	pthread_mutex_lock(&s->mut_array);
	return (who_is_earlier(d1_t, d2_t, data1, data2));
}

bool	edf(void *data1, void *data2)
{
	struct timeval	d1_t;
	struct timeval	d2_t;
	t_span			*s;

	s = (t_span *)((t_coder *) data1)->s;
	if (s->is_begging)
		return (fifo(data1, data2));
	pthread_mutex_unlock(&s->mut_array);
	pthread_mutex_lock(&s->mut_time);
	d1_t = ((t_coder *) data1)->b_interv_s;
	d2_t = ((t_coder *) data2)->b_interv_s;
	pthread_mutex_unlock(&s->mut_time);
	pthread_mutex_lock(&s->mut_array);
	return (who_is_earlier(d1_t, d2_t, data1, data2));
}
