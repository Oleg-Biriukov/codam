#include "ft_printf.h"

int	ft_putptr_fd(void *ptr, int fd)
{
	char	*base;
	int		len;

	base = "0123456789abcdef";
	len = 0;
	if ((uintptr_t) ptr / 16 != 0)
		len = ft_putptr_fd((void *) ((uintptr_t) ptr / 16), fd);
	else
		len += ft_putstr_fd("0x", fd);
	len += ft_putchar_fd(base[(uintptr_t)ptr % 16], fd);
	return (len);
}
