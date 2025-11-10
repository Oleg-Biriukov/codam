/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_isprint.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/07 14:30:20 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/07 14:48:32 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
	{
		return (16384);
	}
	return (0);
}

/*int main()
{
	printf("isprint: %d\nft_isprint: %d", isprint(127), ft_isprint(127));
}*/
