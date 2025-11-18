/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_tolower.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/08 18:27:31 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/13 17:15:48 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_tolower(int c)
{
	if (c >= 65 && c <= 90)
	{
		return (c + 32);
	}
	return (c);
}

/*int main(int argc, char **argv)
{
	if (argc == 1)	return 0;
	printf("ft_tolower: %c\n   tolower: %c\n", 
	tolower(atoi(argv[1])), ft_tolower(atoi(argv[1])));
}*/
