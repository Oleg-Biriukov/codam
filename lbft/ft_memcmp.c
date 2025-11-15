/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_memcmp.c                                         :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/09 10:34:43 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/10 14:46:11 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	while (n--)
	{
		if (*((unsigned char *) s1) != *((unsigned char *) s2))
		{
			return (*((unsigned char *) s1) - *((unsigned char *)s2));
		}
		s1++;
		s2++;
	}
	return (0);
}

/*int main(int argc, char **argv)
{
	if (argc <= 3) return 0;
	printf("   memcmp: %d\nft_memcmp: %d\n", 
	memcmp(argv[1], argv[2], atoi(argv[3])), 
	ft_memcmp(argv[1], argv[2], atoi(argv[3]))); 
}*/
