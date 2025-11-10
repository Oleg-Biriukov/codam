/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_strlcat.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/08 17:50:26 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/14 12:55:12 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <bsd/string.h>

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len;

	if (size < ft_strlen(dst))
		return (ft_strlen(src) + size);
	len = ft_strlen(dst) + ft_strlen(src);
	dst += ft_strlen(dst);
	ft_strlcpy(dst, src, size - (len - ft_strlen(src)));
	dst += size - ft_strlen(dst);
	return (len);
}

/*int main(int argc, char **argv)
{
	if (argc < 3)	return 0;
	char *s1 = malloc(ft_atoi(argv[2]) - 5);
	char *s2 = malloc(ft_atoi(argv[2]) - 5);
	ft_strlcpy(s1, "Oleg ", 6);
	ft_strlcpy(s2, "Oleg ", 6);
	int ft = ft_strlcat(s1, argv[1], ft_atoi(argv[2]));
	int or = strlcat(s2, argv[1], ft_atoi(argv[2]));
	printf("ft_strlcat: %s(%d)\n   strlcat: %s(%d)\n", s1, ft, s2, or);
}*/
