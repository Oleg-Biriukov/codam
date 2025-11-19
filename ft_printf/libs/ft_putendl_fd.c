/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_putendl_fd.c                                     :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/16 13:09:46 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/16 13:51:40 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	int	nextline;

	nextline = '\n';
	write(fd, s, ft_strlen(s));
	write(fd, &nextline, 1);
}
