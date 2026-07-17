/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:12:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/17 16:08:32 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_l(t_array *a)
{
	t_coder *data;

	a = la_start(a);
	while(a != NULL){
		data = (t_coder *) a->data;
		printf("%d", data->id);
		a = a->next;
	}
}

static int	fifo(t_array *a1, t_array *a2)
{
	struct timeval	d1_t;
	struct timeval	d2_t;
	

	d1_t = ((t_coder *) a1->data)->req_t;
	d2_t = ((t_coder *) a2->data)->req_t;
// deciding who made first request for dongle and who made less count of compilations
	if (d1_t.tv_sec * 1000000L + d1_t.tv_usec < d2_t.tv_sec * 1000000L + d2_t.tv_usec)
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
	unsigned int	i;
	t_dongle		*ldata;
	t_dongle		*rdata;
	t_array			*a;
	t_coder			*cdata;

	printf("[%d ms] Awaiting requests for dongles\n", s->time);
	while (1)
	{
		i = 0;
		if (s->is_over || s->is_failed)
			return;
		pthread_mutex_lock(&s->mut);
		la_sort(s->coders, _by);
		pthread_mutex_unlock(&s->mut);
		while (i < la_len(s->coders))
		{	
			a = find_elem(s->workspace, get_elem(s->coders, i++));
			cdata = (t_coder *) a->data;
			rdata = (t_dongle *) (a->prev)->data;
			ldata = (t_dongle *) (a->next)->data;

			pthread_mutex_lock(&s->mut);
			if (cdata->is_done == 1)
			{
				pthread_join(cdata->t, NULL);
				a = get_elem(s->coders, i - 1);
				if (a->next)
					s->coders = a->next;
				else if (a->prev)
					s->coders = a->prev;
				else
					s->coders = NULL;
				la_remove(a);
				
			}
			else if (rdata->is_active == 1 && ldata->is_active == 1)
			{
				cdata->conn[0] = rdata;
				cdata->conn[1] = ldata;
				rdata->is_active = 0;
				ldata->is_active = 0;
				pthread_cond_broadcast(&cdata->cond);
			}
			pthread_mutex_unlock(&s->mut);
			usleep(30);
		}
		
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