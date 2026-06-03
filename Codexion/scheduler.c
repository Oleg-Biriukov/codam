/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:12:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/06/03 16:16:56 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	fifo(t_span *s)
{
	t_array	*coder;
	t_coder	*data_c;
	t_dongle	*data_dl;
	t_dongle	*data_dr;

	coder = s->workspace->prev->prev;
	while (1)
	{
		data_c = (t_coder *) coder->data;
		data_dl = (t_dongle *) coder->next->data;
		data_dr = (t_dongle *) coder->prev->data;
		pthread_mutex_lock(&s->mutex_g);
		printf("[%d ms] REQUEST_DONGLE\tC%d D%d\n", s->time , data_c->id, data_dl->id);
		printf("[%d ms] REQUEST_DONGLE\tC%d D%d\n", s->time , data_c->id, data_dr->id);
		pthread_mutex_unlock(&s->mutex_g);
		pthread_mutex_lock(&s->mutex_cod);
		if (data_dr->is_active &&
			data_dl->is_active &&
			data_dl->is_cooldown == 0 &&
			data_dr->is_cooldown == 0)
		{
			pthread_mutex_unlock(&s->mutex_cod);
			printf("[%d ms] TAKE_DONGLE\tC%d D%d\n", s->time, data_c->id, data_dr->id);
			printf("[%d ms] TAKE_DONGLE\tC%d D%d\n", s->time, data_c->id, data_dl->id);
			pthread_mutex_lock(&s->mutex_cod);
			data_c->conn[0]= coder->prev->data;
			data_c->conn[1]= coder->next->data;
			data_dl->is_active = 0;
			data_dr->is_active = 0;
			pthread_mutex_unlock(&s->mutex_cod);
		}
		pthread_mutex_unlock(&s->mutex_cod);
		coder = coder->prev->prev;
		if (coder == s->workspace->prev->prev)
			break ;
	}
	return (0);
}

static int	edf(t_span *s)
{
	return (la_len(s->coders)); 
}

int scheduler(t_span *s)
{
	if (!strcmp(s->schdlr, "fifo"))
		return (fifo(s));
	if (!strcmp(s->schdlr, "edf"))
		return (edf(s));
	return (-1);
}