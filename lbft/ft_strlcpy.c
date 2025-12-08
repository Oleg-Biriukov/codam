/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_strlcpy.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/08 17:57:24 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/14 12:46:32 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <bsd/string.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	int	len;

	len = ft_strlen(src);
	if (size == 0)
		return (ft_strlen(src));
	while (*src != '\0' && size - 1)
	{
		*(dst++) = *(src++);
		size--;
	}
	*dst = '\0';
	return (len);
}

/*int main(int argc, char **argv)
{
	if (argc < 3)	return 0;
	char *dst = malloc(ft_strlen(argv[1]));
	char *dst1 = malloc(ft_strlen(argv[1]));
	int ft = ft_strlcpy(dst, argv[1], ft_atoi(argv[2]));
	int or = strlcpy(dst1, argv[1], ft_atoi(argv[2]));
	printf("ft_strlcpy: %s(%d)\n   strlcpy: %s(%d)\n", dst, ft, dst1, or);
}*/
