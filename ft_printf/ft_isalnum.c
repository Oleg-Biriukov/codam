/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_isalnum.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/07 13:49:12 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/07 13:59:48 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_isalnum(int c)
{
	if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
	{
		return (8);
	}
	return (0);
}

/*int main()
{
	printf("isalnum: %d\nft_isalnum: %d", ft_isalnum('{'), isalnum('{'));
}*/
