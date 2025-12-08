/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   ft_itoa.c                                           :+:    :+:           */
/*                                                      +:+                   */
/*   By: obirukov <obirukov@student.codam.nl>          +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/16 13:58:33 by obirukov       #+#    #+#                */
/*   Updated: 2025/10/26 16:39:03 by obirukov       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	vton(int value, int n)
{
	int	cval;

	cval = value;
	while (n - 1 > 0)
	{
		value *= cval;
		n--;
	}
	return (value);
}

static size_t	len_i(int n)
{
	size_t	len;

	if (n < 0)
		n *= -1;
	len = 1;
	while ((n / 10) != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static char	*convert(int n, char *num, size_t count)
{
	if (count-- != len_i(n))
	{
		*num++ = '0';
		return (convert(n, num, count));
	}
	if (count == 0)
	{
		*num++ = n + 48;
		*num = '\0';
		return (num);
	}
	*num++ = (n / vton(10, len_i(n) - 1)) + 48;
	return (convert(n % vton(10, len_i(n) - 1), num, count));
}

char	*ft_itoa(int n)
{
	int		i;
	char	*num;

	i = 0;
	if (n < 0)
	{
		if (n == -2147483648)
			return (ft_strdup("-2147483648"));
		n *= -1;
		num = malloc(sizeof(char *) * (len_i(n) + 2));
		if (num == NULL)
			return (NULL);
		*num++ = '-';
		i++;
	}
	else
		num = malloc(sizeof(char *) * (len_i(n) + 1));
	if (num == NULL)
		return (NULL);
	return (convert(n, num, len_i(n)) - len_i(n) - i);
}

/*int main()
{
	char *str = ft_itoa(1000034);
	printf("%s\n", str);
	free(str);
}*/
