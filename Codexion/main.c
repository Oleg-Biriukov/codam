/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:22 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/16 15:24:02 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void free_all(t_span *s)
{
	la_free(s->coders);
	pthread_mutex_destroy(&s->mutex);
}

int main()
{
	t_span *s;

	s = malloc(sizeof(t_span));
	if (!s){
		printf("Something went wrong");
	}
	pthread_mutex_init(&s->mutex, NULL);
	s->n_coders = 3;
	s->t_compile = 2000;
	s->t_refactor = 1000;
	s->d_cooldown = 500;
	s->n_compiles = 2;
	s->t_debug = 1500;
	s->schdlr = "fifo";
	if (!init_coders(s))
		return (0);
	printf("%d", la_len(s->coders));
	return (0);
}
