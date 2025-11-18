/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_strtrim.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/14 17:01:23 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/26 12:46:07 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*start(char *s1, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == *s1)
		{
			ft_memmove((void *) s1, (void *)(s1 + 1), ft_strlen(s1));
			i = 0;
			continue ;
		}
		i++;
	}
	return (s1);
}

static char	*end(char *s1, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == *(s1 + (ft_strlen(s1) - 1)))
		{
			*(s1 + (ft_strlen(s1) - 1)) = '\0';
			i = 0;
			continue ;
		}
		i++;
	}
	return (s1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimstr;

	if (ft_strlen(s1) == 0)
	{
		trimstr = malloc(1);
		if (trimstr == NULL)
			return (NULL);
		*trimstr = '\0';
		return (trimstr);
	}
	trimstr = malloc(sizeof(char *) * (ft_strlen(s1) + 1));
	if (trimstr == NULL)
		return (NULL);
	ft_strlcpy(trimstr, s1, ft_strlen(s1) + 1);
	start(trimstr, set);
	end(trimstr, set);
	return (trimstr);
}

/*int main()
{
	char *str = ft_strtrim("", " ");
	printf("%s\n", str);
	free(str);
}*/
