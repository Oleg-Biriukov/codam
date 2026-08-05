/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:06 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/05 18:12:11 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H

# define CODEXION_H
# include <stdlib.h>
# include <valgrind/valgrind.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <errno.h>
# include <stdbool.h>

typedef struct t_dongle
{
	void			*s;
	bool			is_active;
	bool			is_cooldown;
	pthread_t		t;
	unsigned int	id;
	pthread_cond_t	cond;
	struct timeval	start;
	pthread_mutex_t	mutex;
}	t_dongle;

typedef struct t_coder
{
	void			*s;
	bool			is_done;
	bool			is_active;
	t_dongle		*conn[2];
	pthread_t		t;
	pthread_t		t_burnout;
	unsigned int	id;
	unsigned int	compiles;
	pthread_cond_t	cond;
	struct timeval	req_t;
	struct timeval	b_interv_s;
	struct timeval	b_interv_e;
	struct timeval	start;
	pthread_mutex_t	mutex;
}	t_coder;

typedef struct t_array
{
	void			*data;
	struct t_array	*next;
	struct t_array	*prev;
}	t_array;

typedef struct t_span
{
	pthread_mutex_t	mut;
	pthread_mutex_t	mut_prnt;
	pthread_mutex_t	mut_time;
	pthread_cond_t	c_to_schedule;
	struct timeval	start;
	unsigned int	n_compiles;
	unsigned int	n_coders;
	unsigned int	n_in_progress;
	unsigned int	t_burnout;
	unsigned int	t_compile;
	unsigned int	t_refactor;
	unsigned int	d_cooldown;
	unsigned int	t_debug;
	t_array			*workspace;
	t_array			*coders;
	t_array			*dongle;
	bool			is_over;
	bool			to_schedule;
	bool			is_failed;
	bool			is_burnout;
	char			*schdlr;
	char			**argv;
	int				argc;
}	t_span;

struct timespec	convert(struct timeval from, int b_out);
unsigned int	la_len(t_array *array);
__int64_t		interval(struct timeval s, struct timeval c);
t_array			*la_start(t_array *array);
t_array			*la_append(t_array *array, void *content);
t_array			*la_init(void *content);
t_array			*get_elem(t_array *stack, int num);
t_array			*find_elem(t_array *haystack, t_array *needle);
void			*la_free(t_array *array);
void			la_sort(t_array *a, int (cond)(t_array *, t_array *), t_span *s);
void			take_out_arg(t_span *s, char **argv, int argc);
void			coder(t_coder *data);
void			dongle(t_dongle *data);
void			detect_b(t_coder *data);
void			awaiting_for_signal(t_span *s);
bool			fail(t_span *s);
bool			start(t_span *s);
bool			stages(t_coder *data);
bool			init_arrays(t_span *s);
bool			init_dongle(t_span *s);
bool			scheduler(t_span *s);
bool			wait_check(t_span *s, unsigned int how_many);
int				timer(t_span *s);

#endif
