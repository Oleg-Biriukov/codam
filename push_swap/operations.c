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

	if (la_len(stack_b) < 1)
		return (stack_b);
	push = la_init(stack_b->content);
	if (!push)
		return (push);
	push->next = stack_a;
	stack_a->prev = push;
	get_elem(stack_b, 1)->prev = NULL;
	free(stack_b);
	ft_putstr("pa\n");
	return (push);
}

t_stack	*push_b(t_stack *stack_a, t_stack *stack_b)
{
	t_stack *push;

	if (la_len(stack_a) < 1)
		return (stack_a);
	push = la_init(stack_a->content);
	if (!push)
		return (push);
	push->next = stack_b;
	get_elem(stack_b, 1)->prev = NULL;
	free(stack_b);
	ft_putstr("pb\n");
	return (push);
}
