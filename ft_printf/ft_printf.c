/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_printf.c                                         :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/11/20 18:55:05 by obirukov       #+#    #+#                */
/*   Updated: 2025/11/20 18:55:08 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	convert(char c, va_list args)
{
	int	counter;

	counter = 0;
	if (c == 'd' || c == 'i')
		counter += ft_putnbr_fd(va_arg(args, int), 1);
	else if (c == 's')
		counter += ft_putstr_fd(va_arg(args, char *), 1);
	else if (c == 'c')
		counter += ft_putchar_fd(va_arg(args, int), 1);
	else if (c == '%')
		counter += ft_putchar_fd('%', 1);
	else if (c == 'u')
		counter += ft_putunnbr_fd(va_arg(args, unsigned int), 1);
	else if (c == 'x')
		counter += ft_puthex_fd(va_arg(args, unsigned int), 1, 0);
	else if (c == 'X')
		counter += ft_puthex_fd(va_arg(args, unsigned int), 1, 1);
	else if (c == 'p')
		counter += ft_putptr_fd(va_arg(args, void *), 1);
	else
		return (-1);
	return (counter);
}

int	free_all(va_list args)
{
	va_end(args);
	return (-1);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		counter;
	int		check;

	va_start(args, format);
	counter = 0;
	while (*format != '\0')
	{
		while (*format != '%' && *format != '\0')
			counter += ft_putchar_fd(*(format++), 1);
		if (*format == '\0' || *(format + 1) == '\0')
			break ;
		check = convert(*(format + 1), args);
		if (check < 0)
			return (free_all(args));
		counter += check;
		format += 2;
	}
	if (*format != '\0')
		counter += ft_putstr_fd((char *) format, 1);
	va_end(args);
	return (counter);
}

// int main()
// {
// 	int i, b;
// 	// char s2[20] = "hiiiii\0";
// 	i = printf(" %u %u", INT_MAX, INT_MIN);
// 	printf("\n=========================\n");
// 	b = ft_printf(" %u %u", INT_MAX, INT_MIN);
// 	ft_printf("\n=========================\n");
// 	ft_printf("%d %d", i, b);
// 	// ft_printf("\n=========================\n");
// 	// b = ft_printf("%d%%%%%%\n", INT_MAX, INT_MIN);
// 	// printf("%d\n", b);
// 	return 0;
// }
