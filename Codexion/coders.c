/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:35:19 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/15 15:56:28 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void *smth()
{
	return NULL;
}

void *init_coders(t_span *s)
{
	t_content *data;

	
	data = malloc(sizeof(t_content));
	if (!data)
		return (NULL);
	data->id = 0;
	pthread_create(&data->t, NULL, smth, NULL);
	s->coders = la_init(data);
	if (!s->coders)
		return (NULL);
	while (data->id < s->n_coders){
		data = malloc(sizeof(t_content));
		if (!data)
			return (NULL);
		data->id += 1;
		pthread_create(&data->t, NULL, smth, NULL);
		s->coders = la_append(s->coders, data);
		if (!s->coders)
			return (NULL);
	}
}
