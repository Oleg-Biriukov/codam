/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:12:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/23 14:18:36 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	fifo(t_span *s)
{
	return (la_len(s->coders)); 
}

static int	edf(t_span *s)
{
	return (la_len(s->coders)); 
}

int scheduler(t_span *s)
{
	if (strcmp(s->schdlr, "fifo"))
		return (fifo(s));
	if (strcmp(s->schdlr, "edf"))
		return (edf(s));
	return (-1);
}