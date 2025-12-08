#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
typedef struct	t_stack
{
	struct t_stack	*next;
	struct t_stack	*prev;
	void			*content;
}				t_stack;

t_stack	*la_start(t_stack *array);
t_stack	*la_append(t_stack *array, void *content);
t_stack	*get_elem(t_stack *stack, int num);
t_stack	*la_init(void *content);
t_stack	*push_a(t_stack *stack_a, t_stack *stack_b);
t_stack	*push_b(t_stack *stack_a, t_stack *stack_b);
void 	*la_free(t_stack *array);
int		ft_atoi(const char *nptr);
void	ft_putstr(char *s);
void	swap(t_stack *stack, char mode);
size_t	la_len(t_stack *array);
#endif
