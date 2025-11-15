/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_calloc.c                                         :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/13 20:07:19 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 17:09:39 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	ptr = malloc(nmemb * size);
	if (ptr == NULL || nmemb * size > 4294967295)
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}

/*int main ()
{
	char *pt1 = ft_calloc(0, sizeof(char));
	char *pt2 = calloc(0, sizeof(char));
	printf("ft_calloc: %p\n   calloc: %p\n", pt1, pt2);
	ft_strlcpy(pt1, "Oleh", 5);
	ft_strlcpy(pt2, "Oleh", 5);
	return 0;
}*/
