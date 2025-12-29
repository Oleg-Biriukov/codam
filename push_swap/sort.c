/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 12:25:25 by obirukov          #+#    #+#             */
/*   Updated: 2025/12/29 12:59:48 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static t_span	*shift(t_span *s)
{
	int	calc;

	calc = 1;
	while (!is_biggest(s, s->stack_b, ft_atoi((char *) s->stack_b->content, s)))
	{
		calc++;
		s->stack_b = s->stack_b->next;
	}
	s->stack_b = la_start(s->stack_b);
	if (calc > la_len(s->stack_b) / 2)
		s->rotations[1] = (la_len(s->stack_b) - calc) * -1;
	else
		s->rotations[1] = calc;
	do_smart_rotation(s);
	return (s);
}

static t_span	*push_back_a(t_span *s)
{
	int	calc;

	while (la_len(la_start(s->stack_b)) != 0)
	{
		s->stack_b = la_start(s->stack_b);
		s->stack_a = la_start(s->stack_a);
		s = set_pos(s, s->stack_b, calc_op_a);
		do_smart_rotation(s);
		s = push_a(s, "pa\n");
	}
	s->stack_a = la_start(s->stack_a);
	calc = 0;
	while (!is_smallest(s, s->stack_a,
			ft_atoi((char *) s->stack_a->content, s)))
	{
		calc++;
		s->stack_a = s->stack_a->next;
	}
	s->stack_a = la_start(s->stack_a);
	if (calc > la_len(s->stack_a) / 2)
		s->rotations[0] = (la_len(s->stack_a) - calc) * -1;
	else
		s->rotations[0] = calc;
	return (s);
}

t_span	*sort(t_span *s)
{
	while (la_len(s->stack_a) != 3)
	{
		s = set_pos(s, s->stack_a, calc_op_b);
		do_smart_rotation(s);
		s = push_b(s, "pb\n");
	}
	sort_three(s);
	s = shift(s);
	s = push_back_a(s);
	do_smart_rotation(s);
	return (s);
}
