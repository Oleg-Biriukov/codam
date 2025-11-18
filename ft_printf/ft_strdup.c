/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_strdup.c                                         :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/14 14:37:37 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 16:12:16 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_strdup(const char *s)
{
	char	*dup_s;

	dup_s = malloc(ft_strlen(s) + 1);
	if (dup_s == 0x0)
		return (0x0);
	ft_strlcpy(dup_s, s, ft_strlen(s) + 1);
	return (dup_s);
}

/*int main(int argc, char **argv)
{
	int len = ft_strlen(argv[1]);
	if (argc < 2)	return 0;
	argv[1] += len;
	*(argv[1]) = ' ';
	argv[1] -= len;
	char *s = ft_strdup(argv[1]);
	char *s2 = strdup(argv[1]);
	printf("ft_strdup: %s\n   strdup: %s\n", s, s2);
}*/
