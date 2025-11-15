/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_memchr.c                                         :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/09 08:58:20 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 18:34:55 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (*((unsigned char *) s) == (unsigned char) c)
		{
			return ((void *) s);
		}
		i++;
		s++;
	}
	return (0x0);
}

/*int main(int argc, char **argv)
{
	if (argc == 1)	return 0;
	printf("ft_memchr: %s\n   memchr: %s\n",
	( unsigned char *) ft_memchr(argv[1],'r', 30), 
			(unsigned char *) memchr(argv[1], 'r', 30));
}*/
