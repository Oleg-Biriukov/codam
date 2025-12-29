/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.codam.nl>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-29 09:47:34 by obirukov          #+#    #+#             */
/*   Updated: 2025-12-29 09:47:34 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_biggest(t_span *s, t_stack *stack, int num)
{
	while (stack)
	{
		if (num < ft_atoi((char *) stack->content, s))
			return (0);
		stack = stack->next;
	}
	return (1);
}

int	is_smallest(t_span *s, t_stack *stack, int num)
{
	while (stack)
	{
		if (num > ft_atoi((char *) stack->content, s))
			return (0);
		stack = stack->next;
	}
	return (1);
}

int	is_ascending(t_stack *stack, t_span *s)
{
	int	num;

	stack = la_start(stack);
	while (stack->next)
	{
		num = ft_atoi((char *) stack->content, s);
		if (num > ft_atoi((char *)(stack->next)->content, s))
			return (0);
		stack = stack->next;
	}
	return (1);
}

int	is_deascending(t_stack *stack, t_span *s)
{
	int	num;

	stack = la_start(stack);
	while (stack->next)
	{
		num = ft_atoi((char *) stack->content, s);
		if (num < ft_atoi((char *)(stack->next)->content, s))
			return (0);
		stack = stack->next;
	}
	return (1);
}

t_stack	*get_elem(t_stack *stack, int num)
{
	stack = la_start(stack);
	while (num-- && stack)
		stack = stack->next;
	return (stack);
}
