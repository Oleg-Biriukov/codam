/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:06 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/15 15:53:53 by obirukov         ###   ########.fr       */
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
	pthread_t t;
}t_content;


typedef struct t_coder
{
	void *content;
	struct t_coder *next;
	struct t_coder *prev;
} t_coder;

typedef struct t_span
{
	unsigned int n_coders;
	unsigned int t_burnout;
	unsigned int t_compile;
	unsigned int t_debug;
	unsigned int t_refactor;
	unsigned int n_compiles;
	unsigned int d_cooldown;
	unsigned char *schdlr;
	pthread_mutex_t mutex;
	t_coder *coders;
} t_span;

t_coder		*la_start(t_coder *array);
t_coder		*la_append(t_coder *array, void *content);
t_coder		*la_init(void *content);
void		*init_coders(t_span *s);
int			la_len(t_coder *array);

#endif

