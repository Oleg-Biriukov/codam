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

void	push(t_stack *stack_a, t_stack *stack_b, char mode)
{
	t_stack	*push;

	if (mode == 'a')
	{
		if (la_len(stack_b) < 1)
			return ;
		push = la_init(stack_b->content);
		if (!push)
			return (ft_putstr("Error\n"));
		push->next = stack_a;
		stack_a->prev = push;
		return (ft_putstr("pa\n"));
	}
	if (la_len(stack_a) < 1)
		return ;
	push = la_init(stack_a->content);
	if (!push)
		return (ft_putstr("Error\n"));
	push->next = stack_b;
	return ((ft_putstr("pb\n")));
}
