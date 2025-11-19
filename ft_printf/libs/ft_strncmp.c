/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_strncmp.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/13 16:56:05 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 23:15:34 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int	diff;

	diff = 0;
	while ((*s1 != '\0' || *s2 != '\0') && (n-- > 0))
	{
		diff = (unsigned char) *(s1++) - (unsigned char) *(s2++);
		if (diff != 0)
			return (diff);
	}
	return (diff);
}

/*int main(int argc, char **argv)
{
	if(argc < 4)	return 0;
	printf("ft_strncmp: %s\n   strncmp: %s\n", 
			ft_strncmp(argv[1], argv[2], ft_atoi(argv[3])),
			strncmp(argv[1], argv[2], ft_atoi(argv[3])))
}*/
