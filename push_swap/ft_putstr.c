/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 00:45:18 by obirukov          #+#    #+#             */
/*   Updated: 2025/12/25 00:45:18 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	go_array(t_span *s, t_stack *array, t_stack *elem)
{
	int	n;

	while (array)
	{
		n = ft_atoi((char *) elem->content, s);
		if (ft_atoi((char *) array->content, s) == n && array != elem)
		{
			ft_putstr("Error\n");
			free_all(s);
		}
		array = array->next;
	}
}

void	check_valid(t_span *s)
{
	t_stack	*elem;
	t_stack	*array;
	int		n;

	array = s->stack_a;
	elem = s->stack_a;
	while (elem)
	{
		n = ft_atoi((char *) elem->content, s);
		if (n > INT_MAX || ft_atoi((char *) elem->content, s) < INT_MIN)
		{
			ft_putstr("Error\n");
			free_all(s);
		}
		go_array(s, array, elem);
		array = s->stack_a;
		elem = elem->next;
	}
}

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}

void	ft_putstr(char *s)
{
	write(1, s, ft_strlen(s));
}

void	free_all(t_span *s)
{
	la_free(s->stack_a);
	la_free(s->stack_b);
	free(s->rotations);
	free(s);
	exit(1);
}
