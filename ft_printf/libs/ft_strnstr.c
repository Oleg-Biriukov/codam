/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_strnstr.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/10 18:02:49 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 23:45:14 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	len_l;
	size_t	offset;

	len_l = ft_strlen(little);
	if (*little == 0)
		return ((char *)big);
	offset = 0;
	while (*(big + offset) && offset < len)
	{
		if (len_l + offset > len)
			return (NULL);
		if (*(big + offset) == *little)
		{
			if (ft_strncmp(big + offset, little, len_l) == 0)
				return ((char *)big + offset);
		}
		offset++;
	}
	return (NULL);
}

/*int main()
{
	char haystack[30] = "aaabcabcd";
	char needle[10] = "aabc";
//	ft_strnstr1(haystack, needle, -1);
	printf("ft_strnstr: %s\n   strnstr: %s\n", 
			ft_strnstr("lorem ipsum dolor sit amet", "ipsum", 15), 
			strnstr("lorem ipsum dolor sit amet", "ipsum", 15));
}*/
