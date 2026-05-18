/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:06 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/18 17:26:32 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H

# define CODEXION_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>


typedef struct t_coder
{
	unsigned int 	id;
	pthread_t 	 	t;
	pthread_mutex_t	*mutex;
	struct timeval	start;
	unsigned int 	t_burnout;
	unsigned int 	t_compile;
	unsigned int 	t_debug;
	unsigned int 	t_refactor; 
} t_coder;


typedef struct t_dongle
{
	unsigned int	id;
	unsigned int 	d_cooldown;
	pthread_mutex_t	*mutex;
	struct timeval	start;
	pthread_t		t;
} t_dongle;


typedef struct t_array
{
	void	   	   *data;
	struct t_array *next;
	struct t_array *prev;
} t_array;

typedef struct t_span
{
	unsigned int 	t_burnout;
	unsigned int 	t_compile;
	unsigned int 	t_debug;
	unsigned int 	t_refactor; 
	unsigned int 	n_coders;
	unsigned int 	n_compiles;
	unsigned int 	d_cooldown;
	char 		 	*schdlr;
	t_array 		*coders;
	t_array 		*dongle;
} t_span;

unsigned int	la_len(t_array *array);
t_array			*la_start(t_array *array);
t_array			*la_append(t_array *array, void *content);
t_array			*la_init(void *content);
void			*la_free(t_array *array);
int				init_arrays(t_span *s);
int				init_dongle(t_span *s);

#endif

