/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:22 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/19 17:55:41 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int free_all(t_span *s)
{
	// pthread_mutex_destroy(&(((t_dongle *) s->dongle)->mutex));
	pthread_mutex_destroy(&(((t_coder *) s->coders)->mutex));
	// la_free(s->dongle);
	la_free(s->coders);
	free(s);
	return (0);
}

int main()
{
	t_span 		*s;
	pthread_t 	t[2];
	int			result[2];

	s = malloc(sizeof(t_span));
	if (!s){
		printf("Something went wrong");
	}
	pthread_cond_init(&s->cond, NULL);
	pthread_mutex_init(&s->mutex, NULL);
	s->n_coders = 3;
	s->d_cooldown = 500;
	s->n_compiles = 2;
	s->t_compile = 20000;
	s->t_burnout = 10000;
	s->t_refactor = 3000;
	s->t_debug = 4000;
	s->schdlr = "fifo";

	if (pthread_create(&t[0], NULL, (void *) &init_arrays, s) != 0)
		return(printf("Error"), free_all(s));
	if (pthread_create(&t[1], NULL, (void *) &init_dongle, s) != 0)
		return(printf("Error"), free_all(s));
	pthread_join(t[0], (void *) &result[0]);
	pthread_join(t[1], (void *) &result[1]);
	if (result[0] != 0 )
		return(printf("Error"), free_all(s));
	free_all(s);
	return (0);
}
