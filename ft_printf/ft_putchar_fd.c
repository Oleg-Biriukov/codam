/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_putchar_fd.c                                     :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/11/20 18:54:09 by obirukov       #+#    #+#                */
/*   Updated: 2025/11/20 18:54:12 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar_fd(char c, int fd)
{
	return (write(fd, &c, 1));
}
