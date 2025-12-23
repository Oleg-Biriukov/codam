#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>


typedef struct	t_stack
{
	struct t_stack	*next;
	struct t_stack	*prev;
	void			*content;
	int				pos;
}				t_stack;

typedef struct	t_span
{
	t_stack	*stack_a;
	t_stack	*stack_b;
	int		*rotations;
}				t_span;

typedef int (*t_calc)(t_span *, t_stack *, int);

long int	ft_atoi(const char *nptr, t_span *s);
t_stack		*la_start(t_stack *array);
t_stack		*la_append(t_stack *array, void *content);
t_stack		*la_init(void *content);
t_span		*push_a(t_span *s, char *mode);
t_span		*push_b(t_span *s, char *mode);
t_span		*sort(t_span *s, t_stack *cur, t_calc f_calc);
void 		*la_free(t_stack *array);
void		rev_rotate(t_stack *stack, char *mode);
void		ft_putstr(char *s);
void		do_smart_rotation(t_span *s);
void		rotate(t_stack *stack, char *mode);
void		swap(t_stack *stack, char *mode);
void		free_all(t_span *s);
void		sort_two(t_span *s, t_stack *stack);
void		sort_three(t_span *s);
void		sort_four(t_span *s);
void		sort_five(t_span *s);
void		check_valid(t_span *s);
int			la_len(t_stack *array);
int			calc_op_a(t_span *s, t_stack *stack, int num);
int			calc_op_b(t_span *s, t_stack *stack, int num);
int			is_ascending(t_stack *stack, t_span *s);
int			is_deascending(t_stack *stack, t_span *s);
int			is_smallest(t_span *s, t_stack *stack, int num);
int			is_biggest(t_span *s, t_stack *stack, int num);
int			ft_strlen(char *str);
int			calculation(t_stack *cur, t_stack *pos, t_span *s);

#endif
