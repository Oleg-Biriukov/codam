/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_memcpy.c                                         :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/06 15:41:20 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/28 19:07:56 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	while (n > i)
	{
		((unsigned char *) dest)[i] = ((unsigned char *) src)[i];
		i++;
	}
	return (dest);
}

/*int	main()
{
	char s[40];
	char s1[40];
	ft_memcpy(((void *) 0), ((void *) 0), 0);
	memcpy(s1, "zyxwvutsrqponmlkjihgfedcba", 31);
	printf("   memcpy: %s\nft_memcpy: %s\n", s1, s);
}*/
