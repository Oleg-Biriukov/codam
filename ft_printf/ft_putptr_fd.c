#include "ft_printf.h"

void	ft_putptr_fd(void *ptr, int fd)
{
	char	*base;

	base = "0123456789abcdef";
	if ((uintptr_t) ptr / 16 != 0)
		ft_putptr_fd((void *) ((uintptr_t) ptr / 16), fd);
	else
		ft_putstr_fd("0x", fd);
	ft_putchar_fd(base[(uintptr_t)ptr % 16], fd);
}
