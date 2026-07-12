/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:37:21 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/12 13:43:37 by obirukov         ###   ########.fr       */
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

int	interval(struct timeval s, struct timeval c)
{
	int	diff;

	diff = (c.tv_sec * 1000000L + c.tv_usec) - (s.tv_sec * 1000000L + s.tv_usec);
	return (diff);
}

void	la_sort(t_array *a, int (cond)(t_array *, t_array *))
{
	void	*tmp;
	t_array *ar;
	int     count;

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

void	la_remove(t_array *a)
{
	t_array	*prev;
	t_array	*next;

	prev = a->prev;
	next = a->next;
	free(a->data);
	free(a);
	if (!next && !prev)
		return ;
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
}

// int    cool_down(t_array *a)
// {
//     t_coder *data;

//     data = (t_coder *) a->data;
// 	struct timeval	current_time;
// 	struct timeval	start_time;

	
// 	gettimeofday(&current_time, NULL);
// 	gettimeofday(&start_time, NULL);
// 	while (current_time.tv_sec - start_time.tv_sec < data->left)
// 	{
// 		pthread_mutex_lock(&data->mutex);
// 		if (data->is_done == 1)
// 			return (0);
// 		pthread_mutex_unlock(&data->mutex);
// 		gettimeofday(&current_time, NULL);
// 	}
//     pthread_mutex_lock(&data->mutex);
// 	if (data->is_done == 0)
//     	data->is_burnout = 1;
//     pthread_mutex_unlock(&data->mutex);
//     return (-1);
// }

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
