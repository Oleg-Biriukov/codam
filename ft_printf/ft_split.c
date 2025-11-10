/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_split.c                                          :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/15 16:33:59 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/28 19:13:01 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	len_sub(const char *s, char c)
{
	size_t	i;

	i = 0;
	while (*s != c && *(s++) != '\0')
		i++;
	return (i);
}

static size_t	len_array(const char *s, char c)
{
	size_t	i;

	i = 1;
	while (*s != '\0')
	{
		if (*s == c && *(s + 1) != c && *(s + 1) != '\0')
			i++;
		s++;
	}
	return (i);
}

static void	*free_all(char **s, char *trim_s)
{
	size_t	i;

	i = 0;
	while (s[i] != NULL)
		free(s[i++]);
	free(s);
	free(trim_s);
	return (NULL);
}

static char	**split(char **str, char *trim_s, char c)
{
	int		i;
	size_t	len;

	len = ft_strlen(trim_s);
	i = 0;
	while (*trim_s != '\0')
	{
		while (*trim_s == c)
			trim_s++;
		str[i] = ft_substr(trim_s, 0, len_sub(trim_s, c));
		if (str[i++] == NULL)
		{
			return (free_all(str, trim_s - (len - ft_strlen(trim_s))));
		}
		if (*(trim_s + len_sub(trim_s, c)) == '\0')
			break ;
		trim_s += len_sub(trim_s, c) + 1;
	}
	str[i] = NULL;
	free((void *)(trim_s + len_sub(trim_s, c) - len));
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	char	*trim_s;

	trim_s = ft_strtrim(s, &c);
	if (trim_s == NULL)
		return (NULL);
	str = (char **) ft_calloc(sizeof(char *), len_array(trim_s, c) + 1);
	if (str == NULL)
		return (NULL);
	return (split(str, trim_s, c));
}

/*int main()
{
	char **s = ft_split("Oleh Biriukov", ' ');
	if (s == NULL)
		return (0);
	for(int i = 0; s[i] != NULL; i++){
		printf("%s\n", s[i]);
		free(s[i]);
	}
	free(s);
}*/
