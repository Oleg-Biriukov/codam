/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:12:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/11 17:03:05 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	fifo(t_array *a1, t_array *a2)
{
	struct timeval	d1_t;
	struct timeval	d2_t;
	

	d1_t = ((t_coder *) a1->data)->req_t;
	d2_t = ((t_coder *) a2->data)->req_t;
	if (d1_t.tv_sec * 1000000L + d1_t.tv_usec > d2_t.tv_sec * 1000000L + d2_t.tv_usec)
		return (1);
	return (-1);
}


static int	edf(t_array *a1, t_array *a2)
{
	t_coder	*data1;
	t_coder	*data2;

	data1 = (t_coder *) a1->data;
	data2 = (t_coder *) a2->data;
	return (data1->id + data2->id);
}


static void	scheduling(t_span *s, int (_by)(t_array *, t_array *))
{
	
	printf("[%d ms] Awaiting requests for dongles\n", s->time);
	while (1)
	{
		pthread_mutex_lock(&s->mut);
		la_sort(s->coders, _by);
		pthread_mutex_unlock(&s->mut);
		if (s->is_over || s->is_failed)
			return;
		
	}
}

int scheduler(t_span *s)
{
	if (!strcmp(s->schdlr, "fifo"))
		return (scheduling(s, fifo), 0);
	if (!strcmp(s->schdlr, "edf"))
		return (scheduling(s, edf), 0);
	return (-1);
}