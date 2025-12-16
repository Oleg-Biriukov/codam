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
	s->rotations[0] = 0;
	s->rotations[1] = 0;
}

int calc_op(t_span *s, int num)
{
	int		pos;
	t_stack	*stack;

	stack = la_start(s->stack_b);
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
		if (is_biggest(s, stack, ft_atoi((char *) stack->content, s)) || is_smallest(s, stack, ft_atoi((char *) stack->content, s)))
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
		if (ft_atoi((char *) stack->content, s) == biggest)
		{
			s->rotations[0] += 1;
			return ;
		}
	s->rotations[0] += -1;
	return ;
}
