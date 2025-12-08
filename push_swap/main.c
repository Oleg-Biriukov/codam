
#include "push_swap.h"

void	test(t_stack *stack_a, t_stack *stack_b)
{
	stack_a = la_start(stack_a);
	stack_b = la_start(stack_b);
	printf("a | b\n");
	while(stack_a)
	{
		printf("%d | ", ft_atoi((char *) stack_a->content));
		if (stack_b->content != NULL)
			printf("%d\n", ft_atoi((char *) stack_b->content));
		else
			printf("\n");	
		stack_a = stack_a->next;
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

static int	free_all(t_stack *a, t_stack *b)
{
	la_free(a);
	la_free(b);
	return (0);
}

int main(int argc, char **argv)
{
	t_stack *stack_a;
	t_stack	*stack_b;
	int		i;

	if (argc < 2 || !is_digit(argv[1]))
		return (0);
	stack_a = la_append(NULL, (void *)(argv[1]));
	stack_b = la_append(NULL, NULL);
	i = 2;
	while (i < argc)
	{
		if (!is_digit(argv[i]) || !stack_a || !stack_b)
			return (free_all(stack_a, stack_b));
		stack_a = la_append(stack_a, (void *) argv[i++]);
		stack_b = la_append(stack_b, NULL);
	}
	test(stack_a, stack_b);
	printf("\n======================\n");
	stack_a = push_b(stack_a, stack_b);
	test(stack_a, stack_b);
	return (free_all(stack_a, stack_b));
}