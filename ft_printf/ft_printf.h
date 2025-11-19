/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   libft.h                                             :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/13 17:17:08 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/24 19:11:51 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H_H
# define LIBFTPRINTF_H
# include "libs/libft.h"
# include <stdarg.h>
# include <stdint.h>

typedef struct	s_list
{
	void *content;
	struct s_list *next;
	struct s_list *prev;
}	t_list;

int		ft_printf(const char *format, ...);
void	ft_puthex_fd(unsigned int num, int fd, int mode);
void	ft_putunnbr_fd(size_t n, int fd);
void	ft_putptr_fd(void *ptr, int fd);
t_list	*la_init(void *content);
t_list	*la_start(t_list *array);
t_list	*la_append(t_list *array, void *content);
void	*la_free(t_list *array);
size_t	la_len(t_list *array);
#endif
