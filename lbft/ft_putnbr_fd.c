/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_putnbr_fd.c                                      :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/16 13:58:33 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/26 16:38:57 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	vton(int value, int n)
{
	int	cval;

	cval = value;
	while (n - 1 > 0)
	{
		value *= cval;
		n--;
	}
	return (value);
}

static size_t	len_i(int n)
{
	size_t	len;

	if (n < 0)
		n *= -1;
	len = 1;
	while ((n / 10) != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static void	convert(int n, size_t count, int fd)
{
	if (count-- != len_i(n))
	{
		ft_putchar_fd('0', fd);
		return (convert(n, count, fd));
	}
	if (count == 0)
	{
		ft_putchar_fd(n + 48, fd);
		return ;
	}
	ft_putchar_fd((n / vton(10, len_i(n) - 1)) + 48, fd);
	return (convert(n % vton(10, len_i(n) - 1), count, fd));
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n < 0)
	{
		if (n == -2147483648)
		{
			ft_putstr_fd("-2147483648", fd);
			return ;
		}
		n *= -1;
		ft_putchar_fd('-', fd);
	}
	return (convert(n, len_i(n), fd));
}

/*int main(int argc, char **argv)
{
	if(argc < 2)	return 1;
	ft_putnbr_fd(ft_atoi(argv[1]), 1);
}*/
