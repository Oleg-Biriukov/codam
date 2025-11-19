#include "ft_printf.h"

s_list	*la_init(void *content)
{
	s_list	*array;

	array = malloc(sizeof(s_list));
	if (array == NULL)
		return (NULL);
	array->prev = NULL;
	array->content = content;
	array->next = NULL;
	return (array);
}

s_list	*la_start(s_list *array)
{
	if (array->prev != NULL)
		return (la_start(array->prev));
	return (array);
}

s_list	*la_append(s_list *array, void *content)
{
	s_list	*next_array;

	if (!array)
		return (la_init(content));
	next_array = la_init(content);
	if(!next_array)
		return (NULL);
	array->next = next_array;
	next_array->prev = array;
	return (next_array);
}

void	*la_free(s_list *array)
{
	s_list	*next_array;
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

size_t	la_len(s_list *array)
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
	s_list	*array=NULL;

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
