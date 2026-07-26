/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:37:21 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/26 17:23:24 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_array	*get_elem(t_array *stack, int num)
{
	stack = la_start(stack);
	while (num-- && stack)
		stack = stack->next;
	return (stack);
}

t_array	*find_elem(t_array *haystack, t_array *needle)
{
	while (needle->data != haystack->data && haystack)
		haystack = haystack->next;
	return (haystack);
}

unsigned int	interval(struct timeval s, struct timeval e)
{
	int	diff;

	diff = (e.tv_sec * 1000000L + e.tv_usec)
		- (s.tv_sec * 1000000L + s.tv_usec);
	return (diff);
}

void	la_sort(t_array *a, int (cond)(t_array *, t_array *))
{
	unsigned int     count;
	void			*tmp;
	t_array 		*ar;

    ar = la_start(a);
    count = 0;
	while (1)
	{
	    if (!ar->next)
		{
		    ar = la_start(a);
		    if (la_len(la_start(a)) - 1 == count)
		        break;
		    count = 0;
		}
		if(!cond(ar, ar->next))
		{
			tmp = ar->next->data;
			ar->next->data = ar->data;
			ar->data = tmp;
		}
		else
		    count++;
		ar = ar->next;
	}
}

bool	wait_check(t_span *s, unsigned int how_many)
{
	struct timeval	current_time;
	struct timeval	start_time;
	unsigned int	c_time;

	gettimeofday(&start_time, NULL);
	gettimeofday(&current_time, NULL);
	c_time = interval(start_time, current_time);
	while (c_time < how_many * 1000)
	{
		c_time = interval(start_time, current_time);
		pthread_mutex_lock(&s->mut);
		if (s->is_failed || s->is_over)
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
