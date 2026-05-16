/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:06 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/16 17:01:34 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H

# define CODEXION_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>


typedef struct t_content
{
	unsigned int id;
	pthread_t 	 t;
	void		 *mutex;
} t_content;


typedef struct t_coder
{
	t_content	   *data;
	struct t_coder *next;
	struct t_coder *prev;
} t_coder;

typedef struct t_span
{
	unsigned int 	n_coders;
	unsigned int 	t_burnout;
	unsigned int 	t_compile;
	unsigned int 	t_debug;
	unsigned int 	t_refactor;
	unsigned int 	n_compiles;
	unsigned int 	d_cooldown;
	char 		 	*schdlr;
	pthread_mutex_t mutex;
	t_coder 		*coders;
} t_span;

unsigned int	la_len(t_coder *array);
t_coder			*la_start(t_coder *array);
t_coder			*la_append(t_coder *array, void *content);
t_coder			*la_init(void *content);
void			*la_free(t_coder *array);
int				init_coders(t_span *s);

#endif

