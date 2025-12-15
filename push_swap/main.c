
#include "push_swap.h"

void	test(t_span *s)
{
	t_stack	*a;
	t_stack	*b;

	a = s->stack_a;
	b = s->stack_b;
	if (a)
		a = la_start(a);
	if (b)
		b = la_start(b);
	printf("a | b\n");
	while((a || b))
	{
		if (a != NULL)
			printf("%ld | ", ft_atoi((char *) a->content, s));
		else
			printf("  | ");
		if (b != NULL)
			printf("%ld\n", ft_atoi((char *) b->content, s));
		else
			printf("\n");	
		if (a)
			a = a->next;
		if (b)
			b = b->next;
	}
}

static int	is_digit(char *str)
{
	if (*str == '\0')
	{
		ft_putstr("Error\n");
		return (0);
	}
	while (*str)
	{
		if (!(*str >= 48 && *str <= 57)
			&& *str != 32 && *str != 45)
		{
			ft_putstr("Error\n");
			return (0);
		}
		str++;
	}
	return(1);
}

static t_span	*convert(char *arg, t_span *s)
{
	int	offset;

	offset = 0;
	while (1)
	{
		if ((*arg == '\0' || *arg == ' ') && *(arg + 1) != '\0')
		{
			while (*arg == ' ')
			{
				offset++;
				arg++;
			}
			s->stack_a = la_append(s->stack_a, (void *) arg - offset);
			offset = 0;
			if (!s->stack_a)
				free_all(s);
		}
		if (*arg == '\0')
			break;
		offset++;
		arg++;
	}
	return (s);
}

static t_span	*sort(t_span *s)
{
	int		calc;
	int 	op;
	t_stack	*pos;
	t_stack	*cur;

	s = push_b(s, "pb\n");
	s = push_b(s, "pb\n");
	cur = s->stack_a;
	while (la_len(la_start(cur)) != 3)
	{
		op = INT_MAX;
		while (cur)
		{
			calc = calc_op(s, ft_atoi((char *) cur->content, s));
			if (calc < 0)
				calc *= -1;
			if (op > calc)
			{
				op = calc_op(s, ft_atoi((char *) cur->content, s));
				pos = cur;
				s->rotations[1] = op;
				calc = la_len(la_start(cur)) - (la_len(pos));
				if (op > la_len(la_start(cur)) / 2)
					s->rotations[0] = la_len(pos);
				else
					s->rotations[0] = calc;
			}
			
			cur = cur->next;
		}
		
		do_smart_rotation(s);
		s = push_b(s, "pb\n");
		cur = s->stack_a;
	}
	// sort_three(s);
	// do_smart_rotation(s);
	// sort_three(s);
	// do_smart_rotation(s);
	return (s);
}

void	free_all(t_span *s)
{
	la_free(s->stack_a);
	la_free(s->stack_b);
	free(s->rotations);
	free(s);
	exit(1);
}

int main(int argc, char **argv)
{
	t_span	*s;
	int		i;
	int		*rotations;

	s = malloc(sizeof(t_span));
	s->rotations = (int *) malloc(sizeof(int) * 2);
	s->rotations[0] = INT_MAX;
	s->rotations[1] = INT_MAX;
	if (argc < 2 || !s || !s->rotations)
		return (0);
	i = 1;
	s->stack_b = NULL;
	s->stack_a = NULL;
	while (i < argc)
	{
		if (!is_digit(argv[i]))
			free_all(s);
		s = convert(argv[i], s);
		i++;
	}
	s->stack_a = la_start(s->stack_a);
	check_valid(s);
	// test(s);
	s = sort(s);
	// printf("=================================\n");
	while (!is_deascending(s->stack_b, s))
		rotate(s->stack_b, "");
	// test(s);
	free_all(s);
}

