/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_arg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.codam.nl>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-29 16:26:46 by obirukov          #+#    #+#             */
/*   Updated: 2026-07-29 16:26:46 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_ok(int c)
{
	if ((c >= '0' && c <= '9') || c == 0 || c == ' ')
		return (1);
	return (-1);
}

static int	convert_to_num(t_span *s)
{
	int	return_num;

	if (s->argc == 0)
		return (0);
	return_num = atoi(*s->argv);
	while (**s->argv == ' ')
		(*s->argv)++;
	while (**s->argv != '\0' && **s->argv != ' ')
	{
		if (is_ok(**s->argv) == -1)
			break ;
		else
			(*s->argv)++;
	}
	while (**s->argv == ' ')
		(*s->argv)++;
	if (is_ok(**s->argv) == -1)
		s->is_failed = true;
	s->argc--;
	if (**s->argv == '\0')
		s->argv++;
	return (return_num);
}

void	take_out_arg(t_span *s, char **argv, int argc)
{
	s->argc = argc;
	s->argv = argv;
	s->is_failed = false;
	s->is_over = false;
	s->workspace = NULL;
	s->n_coders = convert_to_num(s);
	s->t_burnout = convert_to_num(s);
	s->t_compile = convert_to_num(s);
	s->t_debug = convert_to_num(s);
	s->t_refactor = convert_to_num(s);
	s->n_compiles = convert_to_num(s);
	s->d_cooldown = convert_to_num(s);
	s->schdlr = *s->argv;
	if (s->argc != 0 || !s->schdlr)
		s->is_failed = true;
	if (s->schdlr)
	{
		if (!strcmp(s->schdlr, "fifo"))
			return ;
		if (!strcmp(s->schdlr, "edf"))
			return ;
		s->is_failed = true;
	}
}
