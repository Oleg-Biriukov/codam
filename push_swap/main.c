/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 00:46:09 by obirukov          #+#    #+#             */
/*   Updated: 2025/12/29 15:51:26 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

// void	test(t_span *s)
// {
// 	t_stack	*a;
// 	t_stack	*b;

// 	a = s->stack_a;
// 	b = s->stack_b;
// 	if (a)
// 		a = la_start(a);
// 	if (b)
// 		b = la_start(b);
// 	printf("%-3sa | b\n", " ");
// 	while((a || b))
// 	{
// 		if (a != NULL)
// 			printf("%-5ld| ", ft_atoi((char *) a->content, s));
// 		else
// 			printf("%5s| ", " ");
// 		if (b != NULL)
// 			printf("%5ld\n", ft_atoi((char *) b->content, s));
// 		else
// 			printf("\n");	
// 		if (a)
// 			a = a->next;
// 		if (b)
// 			b = b->next;
// 	}
// }

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
	return (1);
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
			break ;
		offset++;
		arg++;
	}
	return (s);
}

static void	sort_to_five(t_span *s)
{
	if (la_len(s->stack_a) == 2)
		sort_two(s, s->stack_a);
	else if (la_len(s->stack_a) == 3)
		sort_three(s);
	else if (la_len(s->stack_a) == 4)
		sort_four(s);
	else if (la_len(s->stack_a) == 5)
		sort_five(s);
	do_smart_rotation(s);
	free_all(s);
}

static t_span	*prepatation(t_span *s, char **argv, int argc)
{
	int	i;

	s->stack_b = NULL;
	s->stack_a = NULL;
	s->rotations[0] = 0;
	s->rotations[1] = 0;
	i = 1;
	while (i < argc)
	{
		if (!is_digit(argv[i]))
			free_all(s);
		s = convert(argv[i], s);
		i++;
	}
	s->stack_a = la_start(s->stack_a);
	check_valid(s);
	if (is_ascending(s->stack_a, s))
		free_all(s);
	if (la_len(s->stack_a) <= 5)
		sort_to_five(s);
	s = push_b(s, "pb\n");
	s = push_b(s, "pb\n");
	return (s);
}

int	main(int argc, char **argv)
{
	t_span	*s;

	s = malloc(sizeof(t_span));
	s->rotations = (int *) malloc(sizeof(int) * 2);
	if (argc < 2 || !s || !s->rotations)
		free_all(s);
	s = prepatation(s, argv, argc);
	s = sort(s);
	if (!is_ascending(s->stack_a, s))
		while (1)
			s = s + 1;
	free_all(s);
}
