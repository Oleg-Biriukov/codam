
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 13:04:18 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/15 13:04:19 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

t_array	*la_init(void *data)
{
	t_array	*array;

	array = malloc(sizeof(t_array));
	if (array == NULL)
		return (NULL);
	array->prev = NULL;
	array->data = data;
	array->next = NULL;
	return (array);
}

t_array	*la_start(t_array *array)
{
	if (!array)
		return (NULL);
	if (array->prev != NULL)
		return (la_start(array->prev));
	return (array);
}

t_array	*la_append(t_array *array, void *data)
{
	t_array	*next_array;

	if (!array)
		return (la_init(data));
	next_array = la_init(data);
	if (!next_array)
		return (la_free(array));
	array->next = next_array;
	next_array->prev = array;
	return (next_array);
}

void	*la_free(t_array *array)
{
	t_array	*next_array;

	if (array == NULL)
		return (NULL);
	array = la_start(array);
	while (array->next != NULL)
	{
		next_array = array->next;
		free(array->data);
		free(array);
		array = next_array;
	}
	free(array->data);
	free(array);
	return (NULL);
}

unsigned int	la_len(t_array *array)
{
	size_t	len;

	len = 0;
	if (!array)
		return (0);
	while (array != NULL)
	{
		len++;
		array = array->next;
	}
	return (len);
}
