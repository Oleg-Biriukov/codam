/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   turk_a.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 00:46:37 by obirukov          #+#    #+#             */
/*   Updated: 2025/12/25 00:46:37 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	simple_rot_b(t_span *s)
{
	while (s->rotations[1] != 0)
	{
		if (s->rotations[1] > 0)
		{
			s->rotations[1] -= 1;
			rotate(s->stack_b, "rb\n");
		}
		else if (s->rotations[1] < 0)
		{
			s->rotations[1] += 1;
			rev_rotate(s->stack_b, "rrb\n");
		}
	}
}

static void	simple_rot_a(t_span *s)
{
	while (s->rotations[0] != 0)
	{
		if (s->rotations[0] > 0)
		{
			rotate(s->stack_a, "ra\n");
			s->rotations[0] -= 1;
		}
		else if (s->rotations[0] < 0)
		{
			rev_rotate(s->stack_a, "rra\n");
			s->rotations[0] += 1;
		}
	}
}

void	do_smart_rotation(t_span *s)
{
	if (s->rotations[0] > 0 && s->rotations[1] > 0)
	{
		while (s->rotations[0] != 0 && s->rotations[1] != 0)
		{
			s->rotations[0]--;
			s->rotations[1]--;
			rotate(s->stack_a, "");
			rotate(s->stack_b, "rr\n");
		}
	}
	else if (s->rotations[0] < 0 && s->rotations[1] < 0)
	{
		while (s->rotations[0] != 0 && s->rotations[1] != 0)
		{
			s->rotations[0]++;
			s->rotations[1]++;
			rev_rotate(s->stack_a, "");
			rev_rotate(s->stack_b, "rrr\n");
		}
	}
	simple_rot_a(s);
	simple_rot_b(s);
	s->rotations[0] = 0;
	s->rotations[1] = 0;
}
