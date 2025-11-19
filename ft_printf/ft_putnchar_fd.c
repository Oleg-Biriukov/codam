#include "ft_printf.h"

int	ft_putnchar_fd(const char *str, size_t len)
{
	int	counter;

	counter = 0;
	while (len--)
		counter += ft_putchar_fd(*(str++), 0);
	return (counter);
}