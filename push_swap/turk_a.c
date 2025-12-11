#include "push_swap.h"

void	do_smart_rotation(t_span *s)
{
	int	*rstack_a;
	int	*rstack_b;

	rstack_a = s->rotations;
	rstack_b = s->rotations + 1;
	if (*rstack_a > 0 && *rstack_b > 0)
		while ((*rstack_a-- != 0 || *rstack_b-- != 0))
		{
			rotate(s->stack_a, "");
			rotate(s->stack_b, "rr\n");
		}
	else if (*rstack_a < 0 && *rstack_b < 0)
		while ((*rstack_a++ != 0 || *rstack_b++ != 0))
		{
			rotate(s->stack_a, "");
			rotate(s->stack_b, "rrr\n");
		}
	while ((*rstack_a != 0 && *rstack_b != 0))
	{
		if (*rstack_a > 0)
		{
			rotate(s->stack_a, "ra\n");
			*rstack_a -= 1;
		}
		else
		{
			rev_rotate(s->stack_a, "rra\n");
			*rstack_a += 1;
		}
		if (*rstack_b > 0)
		{
			*rstack_b -= 1;
			rotate(s->stack_b, "rb\n");
		}
		else
		{
			*rstack_b += 1;
			rev_rotate(s->stack_b, "rrb\n");
		}
	}
}

int calc_op(t_stack *stack_b, int num, int *rotations)
{
	int	pos;

	stack_b = la_start(stack_b);
	while (stack_b)
	{
		if (num > ft_atoi((char *) stack_b->content))
			break ;
		stack_b = stack_b->next;
	}
	pos = la_len(la_start(stack_b)) - la_len(stack_b);
	if (pos > la_len(la_start(stack_b)) / 2)
	{
		*(rotations + 1) = la_len(stack_b) * -1;
		return (la_len(stack_b));
	}
	else
	{
		*(rotations + 1) = pos;
		return (pos);
	}
}

int is_ascending(t_stack *stack)
{
	int num;

	stack = la_start(stack);
	while (stack->next->next)
	{
		num = ft_atoi((char *) stack->content);
		if (num > ft_atoi((char *) (stack->next)->content))
			return (0);
		stack = stack->next;
	}
	return (1);
}

int is_deascending(t_stack *stack)
{
	int num;

	stack = la_start(stack);
	while (stack->next->next)
	{
		num = ft_atoi((char *) stack->content);
		if (num < ft_atoi((char *) (stack->next)->content))
			return (0);
		stack = stack->next;
	}
	return (1);
}