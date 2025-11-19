/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_strlen.c                                         :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/06 16:23:45 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/06 16:31:17 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (*s != '\0')
	{
		s++;
		i++;
	}
	return (i);
}

/*int main(int argc, char** argv)
{
	if (argc == 1)	return 0;
	printf("strlen: %lu\tft_strlen: %lu", strlen(argv[1]), ft_strlen(argv[1]));

}*/
