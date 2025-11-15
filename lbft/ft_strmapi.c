/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_strmapi.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/16 11:15:55 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 17:16:00 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	size_t	i;

	i = 0;
	str = malloc(ft_strlen(s) * sizeof(char) + 1);
	if (str == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		str[i] = (char)(f)(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*char test(unsigned int i, char c)
{
	return (c + i);
}

int main(int argc, char **argv)
{
	if (argc < 2)	return 0;
	printf("%s\n", ft_strmapi(argv[1], test));
}*/
