#include "push_swap.h"

void	do_smart_rotation(t_span *s)
{
	if (s->rotations[0] > 0 && s->rotations[1] > 0)
		while (s->rotations[0] != 0 && s->rotations[1] != 0)
		{
			s->rotations[0]--;
			s->rotations[1]--;
			rotate(s->stack_a, "");
			rotate(s->stack_b, "rr\n");
		}
	else if (s->rotations[0] < 0 && s->rotations[1] < 0)
		while (s->rotations[0] != 0 && s->rotations[1] != 0)
		{
			s->rotations[0]++;
			s->rotations[1]++;
			rotate(s->stack_a, "");
			rotate(s->stack_b, "rrr\n");
		}
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

int calc_op(t_span *s, int num)
{
	int	pos;

	s->stack_b = la_start(s->stack_b);
	while (s->stack_b)
	{
		if (num > ft_atoi((char *) s->stack_b->content, s))
			break ;
		s->stack_b = s->stack_b->next;
	}
	pos = la_len(la_start(s->stack_b)) - la_len(s->stack_b);
	if (pos > la_len(la_start(s->stack_b)) / 2)
	{
		*(s->rotations + 1) = la_len(s->stack_b) * -1;
		return (la_len(s->stack_b));
	}
	else
	{
		*(s->rotations + 1) = pos;
		return (pos);
	}
}

int is_ascending(t_stack *stack, t_span *s)
{
	int num;

	stack = la_start(stack);
	while (stack->next)
	{
		num = ft_atoi((char *) stack->content, s);
		if (num > ft_atoi((char *) (stack->next)->content, s))
			return (0);
		stack = stack->next;
	}
	return (1);
}

int is_deascending(t_stack *stack, t_span *s)
{
	int num;

	stack = la_start(stack);
	while (stack->next)
	{
		num = ft_atoi((char *) stack->content, s);
		if (num < ft_atoi((char *) (stack->next)->content, s))
			return (0);
		stack = stack->next;
	}
	return (1);
}

void	sort_three(t_span *s)
{
	int		biggest;
	t_stack	*stack;

	stack = s->stack_a;
	biggest = ft_atoi((char *) stack->content, s);
	if (is_deascending(s->stack_a, s))
	{
		swap(s->stack_a, "sa\n");
		return (rev_rotate(s->stack_a, "rra\n"));
	}
	while (stack->next)
	{
		if ( biggest < ft_atoi((char *) stack->content, s))
			biggest = ft_atoi((char *) stack->content, s);
		stack = stack->next;
	}
	stack = s->stack_a;
	if (ft_atoi((char *) stack->next->content, s) == biggest)
		if (ft_atoi((char *) stack->content, s) < ft_atoi((char *) stack->next->next->content, s))
		{
			swap(s->stack_a, "sa\n");
			return (rotate(s->stack_a, "ra\n"));
		}
	
	if (!is_ascending(s->stack_a, s))
		rotate(s->stack_a, "ra\n");
}
