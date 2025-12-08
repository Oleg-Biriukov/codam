#include "push_swap.h"

t_stack	*get_elem(t_stack *stack, int num)
{
	stack = la_start(stack);
	while (num-- && stack)
		stack = stack->next;
	return (stack);
}

void	swap(t_stack *stack, char mode)
{
	void	*buf;

	if (la_len(stack) < 2)
		return ;
	if (mode == 'a')
		ft_putstr("sa\n");
	else
		ft_putstr("sb\n");
	stack = la_start(stack);
	buf = stack->content;
	stack->content = (get_elem(stack, 1))->content;
	(get_elem(stack, 1))->content = buf;
}

t_stack	*push_a(t_stack *stack_a, t_stack *stack_b)
{
	t_stack	*push;
	t_stack *next;

	stack_a = la_start(stack_a);
	stack_b = la_start(stack_b);
	if (la_len(stack_b) < 1)
		return (stack_b);
	next = stack_b->next;
	push = la_init(stack_b->content);
	if (!push)
		return (push);
	push->next = stack_a;
	stack_a->prev = push;
	next->prev = NULL;
	free(stack_b);
	ft_putstr("pa\n");
	return (next);
}

t_stack	*push_b(t_stack *stack_a, t_stack *stack_b)
{
	t_stack *push;
	t_stack	*next;

	stack_a = la_start(stack_a);
	stack_b = la_start(stack_b);
	if (la_len(stack_a) < 1)
		return (stack_a);
	next = stack_a->next;
	push = la_init(stack_a->content);
	if (!push)
		return (push);
	push->next = stack_b;
	stack_b->prev = push;
	next->prev = NULL;
	free(stack_a);
	ft_putstr("pb\n");
	return (next);
}
