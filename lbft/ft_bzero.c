/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_bzero.c                                          :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/07 18:17:19 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/13 20:27:33 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_bzero(void *s, size_t n)
{
	while (n > 0)
	{
		*((unsigned char *) s) = '\0';
		s++;
		n--;
	}
}

/*int main(int argc, char **argv)
{
	if (argc < 2)	return 0;
	char *s1 = malloc(strlen(argv[1]));
	char *s2 = malloc(strlen(argv[1]));
	memcpy(s1, argv[1], strlen(argv[1]));
	memcpy(s2, argv[1], strlen(argv[1]));
	ft_bzero(s1+1, 4);
	bzero(s2+1, 4);
	printf("   bzero: %s\nft_bzero: %s\n", s2, s1);
}*/
