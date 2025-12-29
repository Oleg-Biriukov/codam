/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   estimate_cheapeast.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 00:45:51 by obirukov          #+#    #+#             */
/*   Updated: 2025/12/25 00:45:51 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	calculation(t_stack *cur, t_stack *pos, t_span *s)
{
	int	calc;

	calc = la_len(la_start(cur)) - (la_len(pos));
	if (calc > la_len(la_start(cur)) / 2)
		return (la_len(pos) * -1);
	return (calc);
}

int	smart_sum(t_span *s, t_stack *cur, t_calc f_calc)
{
	int	op;
	int	calc;

	if (la_start(cur) == la_start(s->stack_a))
		calc = f_calc(s, s->stack_b, ft_atoi((char *) cur->content, s));
	else
		calc = f_calc(s, s->stack_a, ft_atoi((char *) cur->content, s));
	op = calculation(cur, cur, s);
	if (calc <= 0 && op <= 0)
	{
		if (calc * -1 >= op * -1)
			return (calc * -1);
		return (op * -1);
	}
	if (calc >= 0 && op >= 0)
	{
		if (calc >= op)
			return (calc);
		return (op);
	}
	if (calc <= 0 && op >= 0)
		return (calc * -1 + op);
	if (op <= 0 && calc >= 0)
		return (calc + op * -1);
	return (0);
}

static void	set_rot(t_span *s, t_stack *c, t_stack *p, t_calc f_calc)
{
	int	num;

	num = ft_atoi((char *) c->content, s);
	if (la_start(c) == la_start(s->stack_a))
	{
		s->rotations[0] = calculation(c, p, s);
		s->rotations[1] = f_calc(s, s->stack_b, num);
	}
	else
	{
		s->rotations[0] = f_calc(s, s->stack_a, num);
		s->rotations[1] = calculation(c, p, s);
	}
}

t_span	*sort(t_span *s, t_stack *cur, t_calc f_calc)
{
	t_stack			*pos;
	unsigned int	sum;

	sum = UINT_MAX;
	while (cur)
	{
		if (sum > smart_sum(s, cur, f_calc))
		{
			sum = smart_sum(s, cur, f_calc);
			pos = cur;
			set_rot(s, cur, pos, f_calc);
		}
		cur = cur->next;
	}
	return (s);
}
