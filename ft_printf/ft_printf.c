#include "libft.h"

static t_list	*sav_offset(t_list *array, char *format)
{
	while (*format != '\0')
	{
		if (*format == '%')
		{
			array = la_append(array, (void *) format);//free problem
			if (array == NULL)
				return (la_free(array));
			if (*(format + 1) == '%')
				format++;
		}
		format++;
	}
	return (la_start(array));
}

static void	*fr_ee_all(va_list args, t_list *array)
{
	va_end(args);
	la_free(array);
	return (NULL);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	t_list	*array;

	array = sav_offset(NULL, (char *) format);
	for(size_t i = 0; i < la_len(array); i++)
	{
		printf("%s\n", (char *) array->content);
		array = array->next;
	}
	printf("%s\n", format);
	fr_ee_all(args, array);
	return (0);
}

int main()
{
	return (ft_printf("1234%%789%d", 56, 10));
}
