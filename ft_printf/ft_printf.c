#include "ft_printf.h"

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

static int	fr_ee_all(va_list args, t_list *array)
{
	va_end(args);
	la_free(array);
	return (-1);
}

static int	convert(t_list *array, va_list args)
{
	size_t	un_var;
	char	c;

	c = *((char *) array->content + 1);
	if (c == 'd')
		ft_putnbr_fd(va_arg(args, int), 0);
	else if (c == 'i')
		ft_putnbr_fd(va_arg(args, int), 0);
	else if (c == 's')
		ft_putstr_fd(va_arg(args, char *), 0);
	else if (c == 'c')
		ft_putchar_fd(va_arg(args, int), 0);
	else if (c == '%')
		ft_putchar_fd('%', 0);
	else if (c == 'u')
		ft_putunnbr_fd(un_var = va_arg(args, size_t), 0);
	else if (c == 'x')
		ft_puthex_fd(va_arg(args, unsigned int), 0, 0);
	else if (c == 'X')
		ft_puthex_fd(va_arg(args, unsigned int), 0, 1);
	else if (c == 'p')
		ft_putptr_fd(va_arg(args, void *), 0);
	else
		return (-1);
	return (1);
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
	t_list	*beg_array;

	array = sav_offset(NULL, format);
	beg_array = array;
	va_start(args, format);
	while (array != NULL && *format != '\0')
	{
		ft_putnchar_fd((char *) format, array->content - (void *) format);
		if (convert(array, args) == -1)
			return (fr_ee_all(args, beg_array));
		format += array->content - (void *) format + 2;
		array = array->next;
	}
	ft_putstr_fd((char *) format, 0);
	fr_ee_all(args, beg_array);
	return (0);
}

int main()
{
	// int i = 10;

	printf("%#5d", 56);
	printf("\n=========================\n");
	return (ft_printf("cspdiuxX%d%s%d", 10, " 11 12 13 ", 14));
	return 0;
}
