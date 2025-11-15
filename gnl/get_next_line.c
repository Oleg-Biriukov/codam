/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   get_next_line.c                                     :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/11/03 21:37:12 by obirukov       #+#    #+#                */
/*   Updated: 2025/11/15 12:53:33 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	*free_all(char *left, char *new_line)
{
	free(left);
	left = NULL;
	free(new_line);
	return (left);
}

static size_t	ft_strchr(char *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i])
		if (str[i++] == c)
			return (i);
	return (0);
}

static char	*helper(int fd, char *buffer, char *new_line)
{
	static char	*left = NULL;
	int			read_s;

	if (!buffer)
		return (left = free_all(left, new_line));
	ft_bzero(buffer, BUFFER_SIZE + 1);
	read_s = read(fd, buffer, BUFFER_SIZE);
	buffer = ft_strjoin(left, buffer, len(buffer));
	left = NULL;
	if (read_s == -1 || !buffer)
		return (left = free_all(buffer, new_line));
	if (ft_strchr(buffer, '\n') == 0 && len(buffer) != 0)
	{
		new_line = ft_strjoin(new_line, buffer, len(buffer));
		if (!new_line)
			return (left = free_all(left, new_line));
		return (helper(fd, malloc(BUFFER_SIZE + 1), new_line));
	}
	left = ft_strdup(buffer + ft_strchr(buffer, '\n'),
			len(buffer + ft_strchr(buffer, '\n')));
	new_line = ft_strjoin(new_line, buffer, ft_strchr(buffer, '\n'));
	if ((!new_line || !left) || (*left == '\0'
			&& read_s == 0 && *new_line == '\0'))
		return (left = free_all(left, new_line));
	return (new_line);
}

char	*get_next_line(int fd)
{
	char	*nw_line;

	if (BUFFER_SIZE <= 0)
		return (NULL);
	nw_line = helper(fd, malloc(BUFFER_SIZE + 1), NULL);
	if (!nw_line)
		return (NULL);
	return (nw_line);
}

// int main()
// {
// 	int fd = open("test", O_RDONLY);
// 	char	*str;
// 	do{
// 		str = get_next_line(fd);
// 		printf("%s", str);
// 		free(str);
// 	}while(str != NULL);
// 	free(str);
// 	printf("%s", get_next_line(fd));
// 	close(fd);
// }
