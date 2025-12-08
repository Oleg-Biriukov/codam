
#include "push_swap.h"

void	test(t_stack *stack_a, t_stack *stack_b)
{
	if (stack_a)
		stack_a = la_start(stack_a);
	if (stack_b)
		stack_b = la_start(stack_b);
	printf("a | b\n");
	while((stack_a || stack_b))
	{
		if (stack_a != NULL)
			printf("%d | ", ft_atoi((char *) stack_a->content));
		else
			printf("  | ");
		if (stack_b != NULL)
			printf("%d\n", ft_atoi((char *) stack_b->content));
		else
			printf("\n");	
		if (stack_a)
			stack_a = stack_a->next;
		if (stack_b)
			stack_b = stack_b->next;
	}
}

static int	is_digit(char *str)
{
	while (*str)
	{
		if (!(*str >= 48 && *str <= 57))
		{
			ft_putstr("Error\n");
			return (0);
		}
		str++;
	}
	return(1);
}

int	free_all(t_span *s)
{
	la_free(s->stack_a);
	la_free(s->stack_b);
	free(s);
	return (0);
}

int main(int argc, char **argv)
{
	t_span	*s;
	int		i;

	s = malloc(sizeof(t_span));
	if (argc < 2 || !is_digit(argv[1]) || !s)
		return (0);
	s->stack_a = la_append(NULL, (void *)(argv[1]));
	i = 2;
	while (i < argc)
	{
		if (!is_digit(argv[i]) || !s->stack_a)
			return (free_all(s));
		s->stack_a = la_append(s->stack_a, (void *) argv[i++]);
	}
	test(s->stack_a, s->stack_b);
	printf("\n======================\n");
	swap(s->stack_a, "sa\n");
	test(s->stack_a, s->stack_b);
	printf("\n======================\n");
	s = push_b(s, "pb\n");
	s = push_b(s, "pb\n");
	s = push_b(s, "pb\n");
	test(s->stack_a, s->stack_b);
	printf("\n======================\n");
	rotate(s->stack_a, "ra\n");
	rotate(s->stack_b, "rb\n");
	test(s->stack_a, s->stack_b);
	printf("\n======================\n");
	rev_rotate(s->stack_a, "rra\n");
	rev_rotate(s->stack_b, "rrb\n");
	test(s->stack_a, s->stack_b);
	printf("\n======================\n");
	swap(s->stack_a, "sa\n");
	test(s->stack_a, s->stack_b);
	printf("\n======================\n");
	s = push_a(s, "pa\n");
	s = push_a(s, "pa\n");
	s = push_a(s, "pa\n");
	test(s->stack_a, s->stack_b);
	return (free_all(s));
}


