/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_striteri.c                                       :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/16 11:12:52 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 17:51:46 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (i < ft_strlen(s))
	{
		(*f)(i, s + i);
		i++;
	}
}

/*void test(unsigned int i, char *s)
{
	s[i] = ft_tolower(s[i]);
}

int main(int argc, char **argv)
{
	if(argc < 2)	return 0;
	ft_striteri(argv[1], test);
	printf("%s\n", argv[1]);
}*/
