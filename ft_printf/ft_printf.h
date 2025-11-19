/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_printf.h                                             :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/13 17:17:08 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 19:11:51 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H
# include <stdarg.h>
# include <stdint.h>
# include <limits.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct	s_list
{
	void *content;
	struct s_list *next;
	struct s_list *prev;
}	s_list;

int		ft_printf(const char *format, ...);
int		ft_puthex_fd(unsigned int num, int fd, int mode);
int		ft_putunnbr_fd(size_t n, int fd);
int		ft_putptr_fd(void *ptr, int fd);
s_list	*la_init(void *content);
s_list	*la_start(s_list *array);
s_list	*la_append(s_list *array, void *content);
void	*la_free(s_list *array);
size_t	la_len(s_list *array);
int		ft_putnbr_fd(int n, int fd);
int		ft_putstr_fd(char *s, int fd);
int		ft_putchar_fd(char c, int fd);
int		ft_putnchar_fd(const char *str, size_t len);
size_t	ft_strlen(const char *s);
#endif
