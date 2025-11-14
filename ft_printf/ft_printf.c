#include "libft.h"

static t_list	*sav_offset(t_list *array, const char *format)
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

static void	*convert(int c, va_list args)
{
	if (c == 'd')
		ft_putnbr_fd(va_arg(args, int), 0);
	else if (c == 's')
		ft_putstr_fd(va_arg(args, char *), 0);
	else if (c == 'c')
		ft_putchar_fd(va_arg(args, int), 0);
	else if (c == '%')
		ft_putchar_fd('%', 0);
	else if (c == 'u')
		ft_putchar_fd(va_arg(args, unsigned int), 0);
	else
		return (NULL);
	return (args);
}

static void	ft_putnchar_fd(const char *str, size_t len)
{
	while (len--)
		ft_putchar_fd(*(str++), 0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	t_list	*array;

	array = sav_offset(NULL, format);
	va_start(args, format);
	while (array->next != NULL)
	{
		ft_putnchar_fd((char *) format, array->content - (void *) format);
		convert(*((char *) array->content + 1), args);
		format += array->content - (void *) format + 1;
		array = array->next;
	}
	fr_ee_all(args, array);
	return (0);
}

int main()
{
	return (ft_printf("1234%%789%d\n", 56));
}
