/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_puthex_fd.c                                      :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/11/20 18:53:38 by obirukov       #+#    #+#                */
/*   Updated: 2025/11/20 18:53:41 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_puthex_fd(unsigned int num, int fd, int mode)
{
	char	*base;
	int		len;

	len = 0;
	if (mode == 0)
		base = "0123456789abcdef";
	else
		base = "0123456789ABCDEF";
	if (num / 16 != 0)
		len += ft_puthex_fd(num / 16, fd, mode);
	len += ft_putchar_fd(base[num % 16], fd);
	return (len);
}
