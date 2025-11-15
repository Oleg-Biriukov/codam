/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_isdigit.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/07 13:37:50 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/07 13:48:46 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
	{
		return (2048);
	}
	return (0);
}

/*int main()
{
	printf("isdigit: %d\nft_isdigit: %d", isdigit('5'), ft_isdigit('5'));
}*/
