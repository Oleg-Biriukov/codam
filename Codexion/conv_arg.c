/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_arg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obirukov <obirukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 16:26:46 by obirukov          #+#    #+#             */
/*   Updated: 2026/08/20 04:35:19 by obirukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	is_ok(int c)
{
	if ((c >= '0' && c <= '9') || c == 0 || c == ' ')
		return (true);
	return (false);
}

static void	skip_spaces(t_span *s)
{
	while (**s->argv == ' ')
		(*s->argv)++;
}

static int	convert_to_num(t_span *s)
{
	int	return_num;

	if (s->argc == 0)
		return (0);
	if (!*s->argv)
		return (0);
	return_num = atoi(*s->argv);
	if (return_num == 0 && !is_ok(**s->argv))
		s->is_failed = true;
	skip_spaces(s);
	while (**s->argv != '\0' && **s->argv != ' ')
	{
		if (!is_ok(**s->argv))
			break ;
		(*s->argv)++;
	}
	skip_spaces(s);
	if (!is_ok(**s->argv) && s->argc > 1)
		s->is_failed = true;
	else
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
	s->is_burnout = false;
	s->workspace = NULL;
	s->n_coders = convert_to_num(s);
	s->t_burnout = convert_to_num(s);
	s->t_compile = convert_to_num(s);
	s->t_debug = convert_to_num(s);
	s->t_refactor = convert_to_num(s);
	s->n_compiles = convert_to_num(s);
	s->d_cooldown = convert_to_num(s);
	s->to_schedule = true;
	s->schdlr = *s->argv;
	if (!*s->argv || s->argc != 0 || !s->schdlr || s->n_coders > 500)
		s->is_failed = true;
	if (s->schdlr)
	{
		if (!strcmp(s->schdlr, FIFO))
			return ;
		if (!strcmp(s->schdlr, EDF))
			return ;
		s->is_failed = true;
	}
}
