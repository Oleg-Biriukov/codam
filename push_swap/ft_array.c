#include "push_swap.h"

t_stack	*la_init(void *content)
{
	t_stack	*array;

	array = malloc(sizeof(t_stack));
	if (array == NULL)
		return (NULL);
	array->prev = NULL;
	array->content = content;
	array->next = NULL;
	return (array);
}

t_stack	*la_start(t_stack *array)
{
	if (array->prev != NULL)
		return (la_start(array->prev));
	return (array);
}

t_stack	*la_append(t_stack *array, void *content)
{
	t_stack	*next_array;

	if (!array)
		return (la_init(content));
	next_array = la_init(content);
	if(!next_array)
		return (la_free(array));
	array->next = next_array;
	next_array->prev = array;
	return (next_array);
}

void 	*la_free(t_stack *array)
{
	t_stack	*next_array;
	if (array == NULL)
		return (NULL);
	array = la_start(array);
	while (array->next != NULL)
	{
		next_array = array->next;
		free(array);
		array = next_array;
	}
	free(array);
	return (NULL);
}

size_t	la_len(t_stack *array)
{
	size_t	len;

	len = 0;
	array = la_start(array);
	while (array != NULL)
	{
		len++;
		array = array->next;
	}
	return (len);
}
