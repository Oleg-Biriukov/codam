#include "libft.h"

t_list	*la_init(void *content)
{
	t_list	*array;

	array = malloc(sizeof(t_list));
	if (array == NULL)
		return (NULL);
	array->prev = NULL;
	array->content = content;
	array->next = NULL;
	return (array);
}

t_list	*la_start(t_list *array)
{
	if (array->prev != NULL)
		return (la_start(array->prev));
	return (array);
}

t_list	*la_append(t_list *array, void *content)
{
	t_list	*next_array;

	if (!array)
		return (la_init(content));
	next_array = la_init(content);
	if(!next_array)
		return (NULL);
	array->next = next_array;
	next_array->prev = array;
	return (next_array);
}

void	*la_free(t_list *array)
{
	t_list	*next_array;
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

size_t	la_len(t_list *array)
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

/*int main ()
{
	t_list	*array=NULL;

	array = la_append(array, "123");
	if (!array)
		return (0);
	for(int i = 49; i<=57; i++)
	{
		printf("%s\n", (char *)array->content);
		array = la_append(array, &i);
	}
	printf("%ld\n", la_len(array));
	la_free(array);
}*/
