/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_strjoin.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/14 16:30:14 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 16:13:01 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	char	*str;

	len = ft_strlen(s1) + ft_strlen(s2);
	str = malloc(sizeof(char) * len + 1);
	if (str == 0x0)
		return (0x0);
	ft_strlcpy(str, s1, ft_strlen(s1) + 1);
	ft_strlcpy(str + ft_strlen(s1), s2, ft_strlen(s2) + 1);
	str[len] = '\0';
	return (str);
}

/*int main(int argc, char **argv)
{
	if (argc < 3)	return 0;
	printf("%s\n", ft_strjoin(argv[1], argv[2]));
}*/
