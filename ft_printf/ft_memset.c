/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_memset.c                                         :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/07 16:13:49 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 16:26:20 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (n > i)
	{
		*((char *) s) = c;
		i++;
		s++;
	}
	return (s - i);
}

/*int main(int argc, char **argv)
{
	if (argc < 1 || argc > 2)	return 0;
	printf("   memset: %s\nft_memset: %s\n", 
			//"...",
			(unsigned char *)memset(argv[1], '#', -1), 
			//(unsigned char *) ft_memset(argv[1], '#', -1));
			"...");
}*/
