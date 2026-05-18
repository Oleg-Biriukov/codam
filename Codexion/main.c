/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:22 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/17 18:16:10 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int free_all(t_span *s)
{
	pthread_mutex_destroy(&s->dongle->mutex);
	pthread_mutex_destroy(&s->coders->mutex);
	la_free(s->dongle);
	la_free(s->coders);
	free(s);
	return (0);
}

int main()
{
	t_span *s;

	s = malloc(sizeof(t_span));
	if (!s){
		printf("Something went wrong");
	}
	s->n_coders = 3;
	s->d_cooldown = 500;
	s->n_compiles = 2;
	s->t_compile = 2000;
	s->t_burnout = 10000;
	s->t_refactor = 1000;
	s->t_debug = 1500;
	s->schdlr = "fifo";
	if (!init_arrays(s) || !init_dongle(s))
		return (free_all(s));
	printf("%d", la_len(s->coders));
	free_all(s);
	return (0);
}
