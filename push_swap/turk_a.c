#include "push_swap.h"

int calc_op(t_span *s, int num)
{
	int op;
	int pos;

	op = 1;
	pos = 0;
	s->stack_b = la_start(s->stack_b);
	while (s->stack_b)
	{
		if (num < ft_atoi((char *) s->stack_b->content))
			break ;
		s->stack_b = s->stack_b->next;
		pos++;
	}
	op += la_len(la_start(s->stack_b)) - pos;
	if (pos > la_len(la_start(s->stack_b)) / 2)
	{
		pos = 0;
		while (pos++ < op)
			rev_rotate(s->stack_b, "rrb\n");
		return (op);
	}
	else
	{
		op = 0;
		while (op++ <= pos)
			rotate(s->stack_b, "rb\n");
		return (pos + 1);
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