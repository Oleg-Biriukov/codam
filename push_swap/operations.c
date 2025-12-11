#include "push_swap.h"

t_stack	*get_elem(t_stack *stack, int num)
{
	stack = la_start(stack);
	while (num-- && stack)
		stack = stack->next;
	return (stack);
}

void	swap(t_stack *stack, char *mode)
{
	void	*buf;

	if (la_len(la_start(stack)) < 2)
		return ;
	ft_putstr(mode);
	stack = la_start(stack);
	buf = stack->content;
	stack->content = (get_elem(stack, 1))->content;
	(get_elem(stack, 1))->content = buf;
}

t_span	*push_a(t_span *s, char *mode)
{
	t_stack *push;
	t_stack	*next;

	if (la_len(la_start(s->stack_b)) < 1)
		return (s);
	s->stack_b = la_start(s->stack_b);
	next = s->stack_b->next;
	push = la_init(s->stack_b->content);
	if (!push)
		free_all(s);
	ft_putstr(mode);
	if (s->stack_a)
	{
		s->stack_a = la_start(s->stack_a);
		push->next = s->stack_a;
		s->stack_a->prev = push;
	}
	else
		s->stack_a = push;
	if (next)
		next->prev = NULL;
	free(s->stack_b);
	s->stack_b = next;
	return (s);
}

t_span	*push_b(t_span *s, char *mode)
{
	t_stack *push;
	t_stack	*next;

	if (la_len(la_start(s->stack_a)) < 1)
		return (s);
	s->stack_a = la_start(s->stack_a);
	next = s->stack_a->next;
	push = la_init(s->stack_a->content);
	if (!push)
		free_all(s);
	ft_putstr(mode);
	if (s->stack_b)
	{
		s->stack_b = la_start(s->stack_b);
		push->next = s->stack_b;
		s->stack_b->prev = push;
	}
	else
		s->stack_b = push;
	if (next)
		next->prev = NULL;
	free(s->stack_a);
	s->stack_a = next;
	return (s);
}


void	rotate(t_stack *stack, char *mode)
{
	void	*buf;

	if (!stack)
		return ;
	ft_putstr(mode);
	if (la_len(la_start(stack)) < 2)
		return ;
	stack = la_start(stack);
	while (stack)
	{
		buf = stack->content;
		stack->content = (get_elem(stack, 1))->content;
		(get_elem(stack, 1))->content = buf;
		stack = stack->next;
	}
}

void	rev_rotate(t_stack *stack, char *mode)
{
	void	*buf;

	if (!stack)
		return ;
	ft_putstr(mode);
	if (la_len(la_start(stack)) < 2)
		return ;
	stack = get_elem(stack, la_len(la_start(stack)) - 1);
	while (stack->prev)
	{
		buf = stack->content;
		stack->content = (stack->prev)->content;
		(stack->prev)->content = buf;
		stack = stack->prev;
	}
}
