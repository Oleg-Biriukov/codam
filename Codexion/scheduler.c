/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:12:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/29 16:47:05 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	fifo(t_span *s)
{
	t_array	*coder;

	coder = s->workspace->prev->prev;
	while (1)
	{
		if (((t_dongle *) coder->next->data)->is_active &&
			((t_dongle *) coder->prev->data)->is_active &&
			!(((t_coder *) coder->data)->conn[0]) &&
			!(((t_coder *) coder->data)->conn[1]))
		{
			((t_coder *) coder->data)->conn[0]= coder->prev->data;
			((t_coder *) coder->data)->conn[1]= coder->next->data;
			((t_dongle *) coder->prev->data)->is_active = 0;
			((t_dongle *) coder->next->data)->is_active = 0;
		}
		coder = coder->prev->prev;
		if (coder == s->workspace->prev->prev)
			break ;
	}
	pthread_cond_broadcast(&s->cond_b);
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