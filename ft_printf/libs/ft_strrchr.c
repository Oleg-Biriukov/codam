/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_strrchr.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/08 18:42:08 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 18:50:11 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	pos;

	i = 0;
	pos = -1;
	if ((char) c == '\0')
		return ((char *) s + ft_strlen(s));
	while (s[i])
	{
		if (s[i] == (char) c)
		{
			pos = i;
		}
		i++;
	}
	if (pos == -1)
	{
		return (0x0);
	}
	if (s[i] == (char) c)
	{
		return ((char *) s + i);
	}
	return ((char *) s + pos);
}

/*int main(int argc, char **argv)
{
	if (argc == 1)	return 0;
	printf("ft_strrchr: %s\n   strrchr: %s\n", ft_strrchr(argv[1], 32),
       	strrchr(argv[1], 32));
}*/
