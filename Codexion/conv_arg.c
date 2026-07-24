#include "codexion.h"

static int	convert_to_num(t_span *s)
{
	int	return_num;

	return_num = atoi(*s->argv);
	s->argc--;
	if (return_num == 0)
		s->is_failed = 1;
	while (**s->argv != '\0' && **s->argv != ' ')
		(*s->argv)++;
	if (**s->argv == '\0')
		s->argv++;
	return (return_num);
}


void	take_out_arg(t_span *s, char **argv, int argc)
{
	s->argc = argc;
	s->argv = argv;
	s->n_coders = convert_to_num(s);
	s->t_burnout = convert_to_num(s);
	s->t_compile = convert_to_num(s);
	s->t_debug = convert_to_num(s);
	s->t_refactor = convert_to_num(s);
	s->n_compiles = convert_to_num(s);
	s->d_cooldown = convert_to_num(s);
	s->schdlr = *s->argv;

	s->is_failed = 0;
	s->is_over = 0;
	s->workspace = NULL;
	if (s->argc != 1 || !s->schdlr)
		s->is_failed = 1;
}
