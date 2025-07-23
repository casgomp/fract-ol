#include "ft_string.h"

int	ft_atoi(const char *nptr)
{
	unsigned int	i;
	int				sign;
	int				result;

	sign = 1;
	i = 0;
	while (((nptr[i] >= '\t') && (nptr[i] <= '\r')) || (nptr[i] == ' '))
	{
		i++;
	}
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
		{
			sign = -sign;
		}
		i ++;
	}
	result = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = (result * 10) + (nptr[i] - '0');
		i ++;
	}
	return (result * sign);
}
