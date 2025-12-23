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
			rev_rotate(s->stack_a, "");
			rev_rotate(s->stack_b, "rrr\n");
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
	s->rotations[0] = 0;
	s->rotations[1] = 0;
}

int calc_op_b(t_span *s, t_stack *stack, int num)
{
	int		pos;

	stack = la_start(stack);
	if (is_biggest(s, stack, num)) // error
	{
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
	if (is_smallest(s, stack, num))
	{
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
	while (stack->next)
	{
		if (num < ft_atoi((char *) stack->content, s) && num > ft_atoi((char *) stack->next->content, s))
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

int calc_op_a(t_span *s, t_stack *stack, int num)
{
	int	pos;

	stack = la_start(stack);
	if (is_smallest(s, stack, num))
	{
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
	if (is_biggest(s, stack, num))
	{
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
	while (stack->next)
	{
		if (num > ft_atoi((char *) stack->content, s) && num < ft_atoi((char *) stack->next->content, s))
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

