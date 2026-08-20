/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:22 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/20 04:39:02 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	timer(t_span *s)
{
	int				time;
	struct timeval	c_time;

	gettimeofday(&c_time, NULL);
	pthread_mutex_lock(&s->mut_time);
	time = interval(s->start, c_time) / 1000;
	return (pthread_mutex_unlock(&s->mut_time), time);
}

int	free_all(t_span *s)
{
	int		len_workspace;
	t_array	*n_array;

	len_workspace = s->n_coders * 2;
	pthread_mutex_destroy(&s->mut);
	pthread_mutex_destroy(&s->mut_prnt);
	pthread_mutex_destroy(&s->mut_time);
	pthread_mutex_destroy(&s->mut_array);
	pthread_cond_destroy(&s->c_to_schedule);
	la_free(s->dongle);
	la_free(s->coders);
	while (len_workspace--)
	{
		n_array = s->workspace->next;
		free(s->workspace);
		s->workspace = n_array;
	}
	free(s);
	return (0);
}

static int	workspace_init(t_span *s)
{
	t_array	*coders;
	t_array	*start;
	t_array	*dongle;

	coders = s->coders;
	dongle = s->dongle;
	while (la_len(coders) != 0)
	{
		s->workspace = la_append(s->workspace, coders->data);
		if (!s->workspace)
			return (false);
		s->workspace = la_append(s->workspace, dongle->data);
		if (!s->workspace)
			return (false);
		coders = coders->next;
		dongle = dongle->next;
	}
	start = la_start(s->workspace);
	start->prev = s->workspace;
	s->workspace->next = start;
	s->workspace = start;
	return (true);
}

int	main(int argc, char **argv)
{
	t_span		*s;

	s = malloc(sizeof(t_span));
	if (!s || argc == 1 || argc > 9)
		return (printf("Error\n"), free(s), 1);
	take_out_arg(s, ++argv, 7);
	if (s->is_failed)
		return (printf("Error\n"), free(s), 0);
	pthread_mutex_init(&s->mut, NULL);
	pthread_mutex_init(&s->mut_prnt, NULL);
	pthread_mutex_init(&s->mut_time, NULL);
	pthread_mutex_init(&s->mut_array, NULL);
	pthread_cond_init(&s->c_to_schedule, NULL);
	if (!init_arrays(s))
		return (printf("Error\n"), free_all(s));
	if (!init_dongle(s))
		return (printf("Error\n"), free_all(s));
	if (!workspace_init(s))
		return (printf("Error\n"), free_all(s));
	if (!start(s))
		return (printf("Error\n"), free_all(s));
	free_all(s);
	return (0);
}
