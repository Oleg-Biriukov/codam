/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:37:21 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/20 04:16:05 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	fail(t_span *s)
{
	pthread_mutex_lock(&s->mut);
	s->is_failed = true;
	return (pthread_mutex_unlock(&s->mut), false);
}

__int64_t	interval(struct timeval s, struct timeval e)
{
	__int64_t	diff;

	if ((e.tv_sec * 1000000L + e.tv_usec) > (s.tv_sec * 1000000L + s.tv_usec))
		diff = (e.tv_sec * 1000000L + e.tv_usec)
			- (s.tv_sec * 1000000L + s.tv_usec);
	diff = (e.tv_sec * 1000000L + e.tv_usec)
		- (s.tv_sec * 1000000L + s.tv_usec);
	if (diff < 0)
		diff = 0;
	return (diff);
}

bool	wait_check(t_span *s, unsigned int how_many)
{
	struct timeval	current_time;
	struct timeval	start_time;
	unsigned int	c_time;

	gettimeofday(&start_time, NULL);
	gettimeofday(&current_time, NULL);
	c_time = interval(start_time, current_time) / 1000;
	while (c_time <= how_many)
	{
		c_time = interval(start_time, current_time) / 1000;
		pthread_mutex_lock(&s->mut);
		if (s->is_failed || s->is_over || s->is_burnout)
			return (pthread_mutex_unlock(&s->mut), false);
		pthread_mutex_unlock(&s->mut);
		gettimeofday(&current_time, NULL);
		if (RUNNING_ON_VALGRIND)
			usleep(30);
	}
	return (true);
}

struct timespec	convert(struct timeval from, int b_out)
{
	struct timespec	to;

	to.tv_sec = from.tv_sec + (b_out / 1000);
	to.tv_nsec = (from.tv_usec + (b_out % 1000) * 1000) * 1000;
	if (to.tv_nsec >= 1000000000L)
	{
		to.tv_sec += 1;
		to.tv_nsec -= 1000000000L;
	}
	return (to);
}

void	swap(void **a, void **b)
{
	void	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}
