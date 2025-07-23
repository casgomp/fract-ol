#include "ft_printf.h"

int	ft_putstr_prnt(void *str, int *count_chars)
{
	unsigned char	*s;
	int				i;

	if (count_chars == NULL)
		return (-1);
	if (str == NULL)
	{
		if (write(1, "(null)", 6) == -1)
			return (-1);
		(*count_chars) += 6;
		return (0);
	}
	s = (unsigned char *)str;
	i = 0;
	while (s[i])
	{
		if (ft_putchar_prnt((int)s[i], count_chars) == -1)
			return (-1);
		i ++;
	}
	return (0);
}
