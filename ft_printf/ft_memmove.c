/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_memmove.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/07 19:23:33 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/26 17:09:39 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	if (!dest || !src)
		return (NULL);
	if (dest > src)
	{
		while (n > 0)
		{
			*((unsigned char *) dest + (n - 1))
				= *((unsigned char *) src + (n - 1));
			n--;
		}
	}
	else if (dest <= src)
	{
		ft_memcpy(dest, src, n);
	}
	return (dest);
}

/*int main()
{
	char str[] = "OlehBirukov";
	char str1[] = "OlehBirukov";
	ft_memmove(str + 3, str, 5);
	memmove(str1 + 3, str1, 5);
	printf("ft_memmove: %s\n   memmove: %s\n", str, str1);
}*/
