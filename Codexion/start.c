/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 12:33:15 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/26 18:38:20 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	create_coders_t(t_array *a)
{
	t_coder	*c_data;

	c_data = (t_coder *) a->data;
	if (pthread_create(&c_data->t, NULL, (void *) coder, c_data) != 0)
		return (false);
	gettimeofday(&c_data->b_interv_s, NULL);
	if (pthread_create(&c_data->t_burnout,
			NULL, (void *) detect_b, c_data) != 0)
		return (false);
	return (true);
}

static void	create_threads(t_span *s, t_array *a)
{
	unsigned int		counter;
	t_dongle			*d_data;

	counter = 0;
	while (counter != s->n_coders * 2)
	{
		counter++;
		if (counter % 2 == 0)
		{
			d_data = (t_dongle *) a->data;
			if (pthread_create(&d_data->t, NULL, (void *) dongle, d_data) != 0)
				return ((void) fail(s));
		}
		else
			if (!create_coders_t(a))
				return ((void) fail(s));
		a = a->next;
	}
}

static void	awaiting(t_span *s, t_array *a, unsigned int total_c)
{
	unsigned int	counter;
	t_coder			*c_data;

	counter = 0;
	while (1)
	{
		if (counter == total_c)
			break ;
		if (a == s->workspace)
			counter = 0;
		pthread_mutex_lock(&s->mut);
		if (s->is_failed)
			return ((void) pthread_mutex_unlock(&s->mut));
		c_data = (t_coder *) a->data;
		if (c_data->compiles >= s->n_compiles)
			counter += s->n_compiles;
		else
			counter += c_data->compiles;
		pthread_mutex_unlock(&s->mut);
		a = a->next->next;
		if (RUNNING_ON_VALGRIND)
			usleep(30);
	}
}

static void	finish(t_array *a, unsigned int n_coders)
{
	t_coder			*c_data;
	t_dongle		*d_data;
	unsigned int	counter;

	counter = 0;
	while (counter != n_coders * 2)
	{
		counter++;
		if (counter % 2 == 0)
		{
			d_data = (t_dongle *) a->data;
			pthread_join(d_data->t, NULL);
		}
		else
		{
			c_data = (t_coder *) a->data;
			pthread_join(c_data->t, NULL);
			pthread_join(c_data->t_burnout, NULL);
		}
		a = a->next;
	}
}

bool	start(t_span *s)
{
	unsigned int	total_c;
	unsigned int	n_coders;
	t_array			*workspace;
	pthread_t		t;

	workspace = s->workspace;
	create_threads(s, workspace);
	pthread_mutex_lock(&s->mut);
	total_c = s->n_coders * s->n_compiles;
	n_coders = s->n_coders;
	pthread_mutex_unlock(&s->mut);
	if (pthread_create(&t, NULL, (void *) &scheduler, s) != 0)
		fail(s);
	if (!s->is_failed)
		awaiting(s, workspace, total_c);
	pthread_mutex_lock(&s->mut);
	s->is_over = true;
	pthread_mutex_unlock(&s->mut);
	finish(workspace, n_coders);
	pthread_join(t, NULL);
	return (!s->is_failed);
}
