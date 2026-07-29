/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_extended.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.codam.nl>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-29 16:15:43 by obirukov          #+#    #+#             */
/*   Updated: 2026-07-29 16:15:43 by obirukov         ###   ########.fr       */
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

void	la_sort(t_array *a, int (cond)(t_array *, t_array *))
{
	unsigned int	count;
	void			*tmp;
	t_array			*ar;

	ar = la_start(a);
	count = 0;
	while (1)
	{
		if (!ar->next)
		{
			ar = la_start(a);
			if (la_len(la_start(a)) - 1 == count)
				break ;
			count = 0;
		}
		if (!cond(ar, ar->next))
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
