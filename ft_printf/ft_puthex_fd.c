#include "ft_printf.h"

void	ft_puthex_fd(unsigned int num, int fd, int mode)
{
	char			*base;

	if (mode == 0)
		base = "0123456789abcdef";
	else
		base = "0123456789ABCDEF";
	if (num / 16 != 0)
		ft_puthex_fd(num / 16, fd, mode);
	ft_putchar_fd(base[num % 16], fd);
}
