#include "push_swap.h"

int calc_op(t_stack *stack_b, int num)
{
	int op;
	int pos;

	op = 1;
	pos = 0;
	stack_b = la_start(stack_b);
	while (stack_b)
	{
		if (num < ft_atoi((char *) stack_b->content))
			break ;
		stack_b = stack_b->next;
		pos++;
	}
	op += la_len(stack_b) - pos;
	return (op);
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