/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_atoi.c                                           :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/11 19:25:48 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/25 01:50:41 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	vton(int value, int n)
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
	while (*nptr == '0')
		nptr++;
	return (nptr);
}

int	ft_atoi(const char *nptr)
{
	int	value;
	int	i;
	int	sign;

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
		return (0);
	nptr += i - 1;
	while (i - 1 > 0)
	{
		value += (*(nptr - (i - 1)) - 48) * vton(10, i - 1);
		i--;
	}
	value += (nptr[i - 1] - 48);
	return (value * sign);
}

/*int main(int argc, char **argv)
{
	if (argc < 2)	return 0;
	printf("ft_atoi: %d\n   atoi: %d\n",ft_atoi(argv[1]), 
	atoi(argv[1]));
}*/
