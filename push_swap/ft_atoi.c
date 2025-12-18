#include "push_swap.h"

static long int	vton(int value, int n)
{
	int	cval;

	cval = value;
	if (n == 0)
		return (1);
	while (n - 1 > 0)
	{
		value *= cval;
		n--;
	}
	return (value);
}

static const char	*skipspace(const char *nptr)
{
	while (*nptr == 32 || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	return (nptr);
}

static const char	*skipzero(const char *nptr)
{
	int	cond;

	cond = (*nptr == '0' || *(nptr + 1) != 0 || *(nptr + 1) != ' ');
	while (!cond)
	{
		cond = (*nptr == '0' || *(nptr + 1) != 0 || *(nptr + 1) != ' ');
		nptr++;
	}
	return (nptr);
}

static int	out_limit(t_span *s)
{
	ft_putstr("Error\n");
	free_all(s);
	return (0);
}

long int	ft_atoi(const char *nptr, t_span *s)
{
	long int	value;
	int			i;
	int			sign;

	value = 0;
	i = 0;
	sign = 1;
	nptr = skipspace(nptr);
	if (nptr[0] == '-')
		sign = -1;
	if (nptr[i] == '+' || nptr[i] == '-')
		nptr++;
	nptr = skipzero(nptr);
	while (nptr[i] >= 48 && nptr[i] <= 57)
		i++;
	if (i < 1 || i >= 20)
		return (out_limit(s));
	nptr += i - 1;
	while (i - 1 > 0)
	{
		value += (*(nptr - (i - 1)) - 48) * vton(10, i - 1);
		i--;
	}
	value += (nptr[i - 1] - 48);
	return (value * sign);
}
