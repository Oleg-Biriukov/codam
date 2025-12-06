#include "push_swap.h"

static int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}

void	ft_putstr(char *s)
{
	write(1, s, ft_strlen(s));
}