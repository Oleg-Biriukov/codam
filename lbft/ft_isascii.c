/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_isascii.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/07 14:00:47 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/07 14:28:41 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}

/*int main()
{
	printf("isascii: %d\nft_isascii: %d", isascii(128), ft_isascii(128));
}*/
