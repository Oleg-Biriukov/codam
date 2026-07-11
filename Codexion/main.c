/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:22 by obirukov          #+#    #+#             */
/*   Updated: 2026/07/11 14:19:51 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	timer(t_span *s)
{
	struct timeval	c_time;
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	while (s->is_over != 1)
	{
		if (s->is_failed == 1)
			return ;
		gettimeofday(&c_time, NULL);
		pthread_mutex_lock(&s->mut);
		s->time = (c_time.tv_sec * 1000000L + c_time.tv_usec) - (s_time.tv_sec * 1000000L + s_time.tv_usec);
		s->time /= 1000;
		pthread_mutex_unlock(&s->mut);
	}
}

int free_all(t_span *s)
{
	int		len_workspace;
	t_array	*n_array;

	len_workspace = s->n_coders * 2;
	pthread_mutex_destroy(&s->mut);
	// pthread_mutex_destroy(&s->mutex_cond);
	pthread_cond_destroy(&s->cond_next);
	pthread_cond_destroy(&s->cond_cod);
	la_free(s->dongle);
	la_free(s->coders);
	while (len_workspace--)
	{
		n_array = s->workspace->next;
		free(s->workspace);
		s->workspace = n_array;
	}
	free(s);
	return (0);
}

static int	workspace_init(t_span *s)
{
	t_array	*coders;
	t_array *start;
	t_array	*dongle;

	coders = s->coders;
	dongle = s->dongle;
	while (la_len(coders) != 0)
	{
		s->workspace = la_append(s->workspace, coders->data);
		if (!s->workspace)
			return (-1);
		s->workspace = la_append(s->workspace, dongle->data);
		if (!s->workspace)
			return (-1);
		coders = coders->next;
		dongle = dongle->next;
	}
	start = la_start(s->workspace);
	start->prev = s->workspace;
	s->workspace->next = start;
	s->workspace = start;
	return (0);
}

int main()
{
	t_span 		*s;
	pthread_t 	t;

	s = malloc(sizeof(t_span));
	if (!s){
		printf("Something went wrong");
	}
	pthread_cond_init(&s->cond_next, NULL);
	pthread_cond_init(&s->cond_cod, NULL);
	pthread_mutex_init(&s->mut, NULL);
	// pthread_mutex_init(&s->mutex_cond, NULL);
	s->n_coders = 2;
	s->d_cooldown = 500;
	s->n_compiles = 1;
	s->t_compile = 1000;
	s->t_burnout = 10000;
	s->t_refactor = 3000;
	s->t_debug = 4000;
	s->is_failed = 0;
	s->is_over = 0;
	s->n_in_progress = 0;
	s->schdlr = "fifo";
	s->workspace = NULL;

	if (init_arrays(s) != 0)
		return (printf("Error"), free_all(s));
	if (init_dongle(s) != 0)
		return (printf("Error"), free_all(s));
	if (workspace_init(s) !=0)
		return (printf("Error"), free_all(s));
	if (pthread_create(&t, NULL, (void *) &timer, s) != 0)
		return (printf("Error"), free_all(s));
	if (start(s) != 0)
		return(printf("Error"), free_all(s));
	if (pthread_join(t, NULL) != 0)
		return (printf("Error"), free_all(s));
	free_all(s);
	return (0);
	}
		
		// t_array *a = s->coders;
		// for (unsigned int i = 0; i < s->n_coders; i++){
		// 	if (((t_coder *) a->data)->conn[0] != NULL  &&
		// 		((t_coder *) a->data)->conn[1] != NULL)
		// 		printf("C%d -> {D%d, D%d}\n", ((t_coder *)a->data)->id, ((t_coder *)a->data)->conn[0]->id, ((t_coder *)a->data)->conn[1]->id);
		// 	a = a->next;
		// }