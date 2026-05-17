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
	s->d_cooldown = 500;
	s->n_compiles = 2;
	s->schdlr = "fifo";
	if (!init_coders(s))
		return (0);
	printf("%d", la_len(s->coders));
	return (0);
}
