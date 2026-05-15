
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

t_coder	*la_init(void *content)
{
	t_coder	*array;

	array = malloc(sizeof(t_coder));
	if (array == NULL)
		return (NULL);
	array->prev = NULL;
	array->content = content;
	array->next = NULL;
	return (array);
}

t_coder	*la_start(t_coder *array)
{
	if (!array)
		return (NULL);
	if (array->prev != NULL)
		return (la_start(array->prev));
	return (array);
}

t_coder	*la_append(t_coder *array, void *content)
{
	t_coder	*next_array;

	if (!array)
		return (la_init(content));
	next_array = la_init(content);
	if (!next_array)
		return (la_free(array));
	array->next = next_array;
	next_array->prev = array;
	return (next_array);
}

void	*la_free(t_coder *array)
{
	t_coder	*next_array;

	if (array == NULL)
		return (NULL);
	array = la_start(array);
	while (array->next != NULL)
	{
		next_array = array->next;
		free(array);
		free(array->content);
		array = next_array;
	}
	free(array);
	free(array->content);
	return (NULL);
}

int	la_len(t_coder *array)
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
