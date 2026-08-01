/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:22 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/01 13:35:04 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	timer(t_span *s)
{
	struct timeval	c_time;
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	while (1)
	{
		gettimeofday(&c_time, NULL);
		pthread_mutex_lock(&s->mut);
		if (s->is_failed || s->is_over || s->is_burnout)
		{
			pthread_mutex_unlock(&s->mut);
			return ;
		}
		s->time = (c_time.tv_sec * 1000000L + c_time.tv_usec)
			- (s_time.tv_sec * 1000000L + s_time.tv_usec);
		s->time /= 1000;
		pthread_mutex_unlock(&s->mut);
		if (RUNNING_ON_VALGRIND)
			usleep(30);
	}
}

int	free_all(t_span *s)
{
	int		len_workspace;
	t_array	*n_array;

	len_workspace = s->n_coders * 2;
	pthread_mutex_destroy(&s->mut);
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
	pthread_t	t;

	s = malloc(sizeof(t_span));
	if (!s || argc == 1)
		return (printf("Error\n"));
	take_out_arg(s, ++argv, 7);
	if (s->is_failed)
		return (printf("Error\n"), free(s), 0);
	pthread_mutex_init(&s->mut, NULL);
	if (!init_arrays(s))
		return (printf("Error\n"), free_all(s));
	if (!init_dongle(s))
		return (printf("Error\n"), free_all(s));
	if (!workspace_init(s))
		return (printf("Error\n"), free_all(s));
	if (pthread_create(&t, NULL, (void *) &timer, s) != 0)
		return (printf("Error\n"), free_all(s));
	if (!start(s))
		return (printf("Error\n"), pthread_join(t, NULL),free_all(s));
	pthread_join(t, NULL);
	free_all(s);
	return (0);
}
