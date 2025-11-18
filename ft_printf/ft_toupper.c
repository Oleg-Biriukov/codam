/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_toupper.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/08 18:11:35 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/08 18:26:49 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
	{
		return (c - 32);
	}
	return (c);
}

/*int main(int argc, char **argv)
{
	if (argc == 1)	return 0;
	printf("   toupper: %c\nft_toupper: %c\n",
		       //'c',	
			toupper(atoi(argv[1])),
		       //'c',	
			ft_toupper(atoi(argv[1])));
}*/
