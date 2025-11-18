/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_strchr.c                                         :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/08 18:42:08 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 18:52:31 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char) c)
		{
			return ((char *) s + i);
		}
		i++;
	}
	if (s[i] == (char) c)
	{
		return ((char *) s + i);
	}
	return (0x0);
}

/*int main(int argc, char **argv)
{
	if (argc == 1)	return 0;
	printf("ft_strchr: %s\n   strchr: %s\n", ft_strchr(argv[1], 
	'g'), strchr(argv[1], 'g'));
}*/
