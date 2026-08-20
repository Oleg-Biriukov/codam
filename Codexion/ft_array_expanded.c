/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_expanded.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 16:15:43 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/20 03:54:32 by obirukov         ###   ########.fr       */
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
