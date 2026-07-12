/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:06 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/12 13:44:28 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H

# define CODEXION_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <errno.h>

typedef struct t_dongle
{
	unsigned int	id;
	unsigned int	is_cooldown;
	void			*s;
	struct timeval	start;
	pthread_t		t;
	int				is_active;
} t_dongle;

typedef struct t_coder
{
	void			*s;
	t_dongle		*conn[2];
	pthread_t		t;
	unsigned int 	id;
	unsigned int	is_done;
	unsigned int	is_burnout;
	unsigned int	compiles;
	pthread_cond_t	cond;
	struct timeval	req_t;
	struct timeval	start;
} t_coder;

typedef struct t_array
{
	void	   	   *data;
	struct t_array *next;
	struct t_array *prev;
} t_array;

typedef struct t_span
{
	// pthread_mutex_t	mutex_cond;
	pthread_mutex_t	mut;
	pthread_cond_t	cond_next;
	pthread_cond_t	cond_cod;
	unsigned int	time;
	unsigned int	is_over;
	unsigned int	is_ready;
	unsigned int	is_failed;
	t_array			*workspace;
	t_array 		*coders;
	t_array 		*dongle;
	char 		 	*schdlr;
	int 			n_coders;
	int 			n_in_progress;
	int			 	t_burnout;
	int			 	t_compile;
	int			 	t_refactor; 
	int			 	n_compiles;
	int			 	d_cooldown;
	int			 	t_debug;
} t_span;

struct timespec	convert(struct timeval from, int b_out);
t_array			*la_start(t_array *array);
t_array			*la_append(t_array *array, void *content);
t_array			*la_init(void *content);
t_array			*get_elem(t_array *stack, int num);
t_array			*find_elem(t_array *haystack, t_array *needle);
void			*la_free(t_array *array);
void			set_to_null(t_coder *data);
void			la_sort(t_array *a, int (cond)(t_array *, t_array *));
void			la_remove(t_array *a);
void			coder(t_coder *data);
void			dongle(t_dongle *data);
int    			check_burnout(t_span *s);
int				la_len(t_array *array);
int 			fail(t_span *s);
int    			cool_down_dongle(t_span *s);
int				start(t_span *s);
int				stages(t_array *array);
int				proccess(t_array *array);
int				init_arrays(t_span *s);
int				init_dongle(t_span *s);
int				scheduler(t_span *s);
int				interval(struct timeval s, struct timeval c);

#endif

