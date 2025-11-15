/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_isalpha.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/06 15:37:52 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/06 15:38:35 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
	{
		return (1024);
	}
	return (0);
}

/*int	main()
{
	printf("islpha(d) %d\n", isalpha('d'));
	printf("islpha(2) %d\n", isalpha('2'));
	printf("ft_islpha(d) %d\n", ft_isalpha('d'));
	printf("ft_islpha(2) %d\n", ft_isalpha('2'));
}*/
