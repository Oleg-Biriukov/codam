/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 12:14:22 by obirukov          #+#    #+#             */
/*   Updated: 2026/05/15 15:54:58 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int main()
{
	t_span *s;

	s = malloc(sizeof(t_span));
	if (!s){
		printf('Something went wrong');
	}
	s->n_coders = 3;
	s->t_compile = 2000;
	s->t_refactor = 1000;
	s->d_cooldown = 500;
	s->n_compiles = 2;
	s->t_debug = 1500;
	s->schdlr = "fifo";
	pthread_mutex_init(&s->mutex, NULL);
	if (!init_coders(s->coders))
		return 0;
	printf(la_len(s->coders));
}
