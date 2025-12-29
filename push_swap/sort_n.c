/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_n.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 00:46:30 by obirukov          #+#    #+#             */
/*   Updated: 2025/12/29 12:40:59 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	sort_by_biggest(t_span *s, int biggest, int smallest)
{
	t_stack	*stack;

	stack = s->stack_a;
	if (ft_atoi((char *) stack->next->content, s) == biggest
		&& ft_atoi((char *) stack->next->next->content, s) != smallest)
	{
		s->rotations[0] += -1;
		return (swap(s->stack_a, "sa\n"));
	}
	if (ft_atoi((char *) stack->content, s) != smallest
		&& ft_atoi((char *) stack->next->next->content, s) == biggest)
		return (swap(s->stack_a, "sa\n"));
	if (!is_ascending(s->stack_a, s))
	{
		if (ft_atoi((char *) stack->content, s) == biggest)
		{
			s->rotations[0] += 1;
			return ;
		}
	}
	s->rotations[0] += -1;
	return ;
}

void	sort_two(t_span *s, t_stack *stack)
{
	if (is_ascending(stack, s))
		return ;
	if (stack == s->stack_a)
		s->rotations[0] += 1;
	if (stack == s->stack_b)
		s->rotations[1] += 1;
}

void	sort_three(t_span *s)
{
	int		biggest;
	int		smallest;
	int		cur;
	t_stack	*stack;

	stack = s->stack_a;
	if (is_deascending(s->stack_a, s))
	{
		swap(s->stack_a, "sa\n");
		s->rotations[0] += -1;
		return ;
	}
	if (is_ascending(s->stack_a, s))
		return ;
	while (stack)
	{
		cur = ft_atoi((char *) stack->content, s);
		if (is_biggest(s, s->stack_a, cur))
			biggest = ft_atoi((char *) stack->content, s);
		if (is_smallest(s, s->stack_a, cur))
			smallest = ft_atoi((char *) stack->content, s);
		stack = stack->next;
	}
	return (sort_by_biggest(s, biggest, smallest));
}

void	sort_four(t_span *s)
{
	int		i;
	t_stack	*stack;

	i = 0;
	s = push_b(s, "pb\n");
	sort_three(s);
	s = set_pos(s, s->stack_b, calc_op_a);
	do_smart_rotation(s);
	s = push_a(s, "pa\n");
	if (!is_ascending(s->stack_a, s))
	{
		stack = s->stack_a;
		while (!is_smallest(s, stack, ft_atoi((char *) stack->content, s)))
			stack = stack->next;
		i = la_len(s->stack_a) - la_len(stack);
		if (i > la_len(s->stack_a) / 2)
			s->rotations[0] = la_len(stack) * -1;
		else
			s->rotations[0] = i + 1;
		do_smart_rotation(s);
	}
}

void	sort_five(t_span *s)
{
	int		i;
	t_stack	*stack;

	i = 0;
	s = push_b(s, "pb\n");
	s = push_b(s, "pb\n");
	sort_three(s);
	s = set_pos(s, s->stack_b, calc_op_a);
	do_smart_rotation(s);
	s = push_a(s, "pa\n");
	s = set_pos(s, s->stack_b, calc_op_a);
	do_smart_rotation(s);
	s = push_a(s, "pa\n");
	if (!is_ascending(s->stack_a, s))
	{
		stack = s->stack_a;
		while (!is_smallest(s, stack, ft_atoi((char *) stack->content, s)))
			stack = stack->next;
		i = la_len(s->stack_a) - la_len(stack);
		if (i > la_len(s->stack_a) / 2)
			s->rotations[0] = la_len(stack) * -1;
		else
			s->rotations[0] = i + 1;
		do_smart_rotation(s);
	}
}
