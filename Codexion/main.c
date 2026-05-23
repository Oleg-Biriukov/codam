/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:22 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/23 17:12:45 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int free_all(t_span *s)
{
	pthread_mutex_destroy(&(((t_dongle *) s->dongle)->mutex));
	pthread_mutex_destroy(&(((t_coder *) s->coders)->mutex));
	pthread_cond_destroy(&s->cond);
	la_free(s->dongle);
	la_free(s->coders);
	while (s->n_coders--)
	{
		free(s->workspace);
		s->workspace = s->workspace->next;
		free(s->workspace);
		s->workspace = s->workspace->next;
	}
	free(s);
	return (0);
}

static int	workspace_init(t_span *s)
{
	t_array	*coders;
	t_array *start;
	t_array	*dongle;

	coders = s->coders;
	dongle = s->dongle;
	while (la_len(coders) != 0)
	{
		s->workspace = la_append(s->workspace, coders->data);
		if (!s->workspace)
			return (-1);
		s->workspace = la_append(s->workspace, dongle->data);
		if (!s->workspace)
			return (-1);
		coders = coders->next;
		dongle = dongle->next;
	}
	start = la_start(s->workspace);
	start->prev = s->workspace;
	s->workspace->next = start;
	s->workspace = start;
	return (0);
}

int main()
{
	t_span 		*s;
	// pthread_t 	t[2];
	// int			result[2];

	s = malloc(sizeof(t_span));
	if (!s){
		printf("Something went wrong");
	}
	pthread_cond_init(&s->cond, NULL);
	pthread_mutex_init(&s->mutex, NULL);
	s->n_coders = 3;
	s->d_cooldown = 500;
	s->n_compiles = 2;
	s->t_compile = 20000;
	s->t_burnout = 10000;
	s->t_refactor = 3000;
	s->t_debug = 4000;
	s->schdlr = "fifo";
	s->workspace = NULL;

	if (init_arrays(s) != 0)
		return (printf("Error"), free_all(s));
	if (init_dongle(s) != 0)
		return (printf("Error"), free_all(s));
	if (workspace_init(s) !=0)
		return (printf("Error"), free_all(s));
	
	
	// if (scheduler(s) != 0)
		// return(printf("Error"), free_all(s));
	free_all(s);
	return (0);
}
