/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   get_next_line_utils->c                               :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/28 13:39:39 by obirukov       #+#    #+#                */
/*   Updated: 2025/11/15 12:54:15 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	len(char *str)
{
	size_t	len;

	len = 0;
	if (!str)
		return (len);
	while (str[len])
		len++;
	return (len);
}

void	*ft_bzero(char *str, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
		str[i++] = '\0';
	return (str);
}

char	*ft_strjoin(char *s1, char *s2, size_t size)
{
	size_t	slen;
	char	*str;

	slen = len(s1) + size;
	str = malloc(sizeof(char) * (slen + 1));
	if (str == 0x0)
	{
		free(s1);
		free(s2);
		return (0x0);
	}
	ft_strlcpy(str, s1, len(s1) + 1);
	ft_strlcpy(str + len(s1), s2, size + 1);
	str[slen] = '\0';
	free(s1);
	free(s2);
	return (str);
}

size_t	ft_strlcpy(char *dst, char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (len(src));
	while (i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len(src));
}

char	*ft_strdup(char *s, int len_s)
{
	char	*dup_s;

	if (len_s < 0)
		len_s = 0;
	dup_s = malloc(len_s + 1);
	if (dup_s == 0x0)
		return (0x0);
	ft_strlcpy(dup_s, s, (size_t) len_s + 1);
	return (dup_s);
}
