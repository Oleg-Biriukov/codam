/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.codam.nl>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-12-29 09:17:02 by obirukov          #+#    #+#             */
/*   Updated: 2025-12-29 09:17:02 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "push_swap.h"

static int	calc_big_small(t_span *s, t_stack *stack)
{
	int	pos;

	if (is_biggest(s, stack, ft_atoi((char *) stack->content, s)))
		return (0);
	while (!is_biggest(s, stack, ft_atoi((char *) stack->content, s)))
		stack = stack->next;
	pos = la_len(la_start(stack)) - (la_len(stack));
	if (pos > la_len(stack) / 2)
		return ((la_len(stack)) * -1);
	else
		return (pos);
}

int	calc_op_b(t_span *s, t_stack *stack, int num)
{
	int		pos;

	stack = la_start(stack);
	if (is_biggest(s, stack, num))
		return (calc_big_small(s, stack));
	if (is_smallest(s, stack, num))
		return (calc_big_small(s, stack));
	while (stack->next)
	{
		if (num < ft_atoi((char *) stack->content, s)
			&& num > ft_atoi((char *) stack->next->content, s))
			break ;
		stack = stack->next;
	}
	pos = la_len(la_start(stack)) - (la_len(stack) - 1);
	if (pos == 0)
		return (1);
	else if (pos > la_len(la_start(stack)) / 2)
		return ((la_len(stack) - 1) * -1);
	return (pos);
}

static int	if_is_smallest(t_span *s, t_stack *stack)
{
	int	pos;

	if (is_smallest(s, stack, ft_atoi((char *) stack->content, s)))
		return (0);
	while (!is_smallest(s, stack, ft_atoi((char *) stack->content, s)))
		stack = stack->next;
	pos = la_len(la_start(stack)) - (la_len(stack));
	if (pos == 0)
		return (0);
	if (pos > la_len(la_start(stack)) / 2)
		return (-1 * (la_len(stack)));
	else
		return (pos);
}

static int	if_is_biggest(t_span *s, t_stack *stack)
{
	int	pos;

	while (!is_biggest(s, stack, ft_atoi((char *) stack->content, s)))
		stack = stack->next;
	pos = la_len(la_start(stack)) - (la_len(stack) - 1);
	if (pos == 0)
		return (0);
	if (pos > la_len(la_start(stack)) / 2)
		return (-1 * (la_len(stack) - 1));
	else
		return (pos);
}

int	calc_op_a(t_span *s, t_stack *stack, int num)
{
	int	pos;

	stack = la_start(stack);
	if (is_smallest(s, stack, num))
		return (if_is_smallest(s, stack));
	if (is_biggest(s, stack, num))
		return (if_is_biggest(s, stack));
	while (stack->next)
	{
		if (num > ft_atoi((char *) stack->content, s)
			&& num < ft_atoi((char *) stack->next->content, s))
			break ;
		stack = stack->next;
	}
	pos = la_len(la_start(stack)) - (la_len(stack) - 1);
	if (pos == 0)
		return (1);
	else if (pos > la_len(la_start(stack)) / 2)
		return ((la_len(stack) - 1) * -1);
	return (pos);
}
