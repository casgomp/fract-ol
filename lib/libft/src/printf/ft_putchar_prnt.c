#include "ft_printf.h"

int	ft_putchar_prnt(int c, int *count_chars)
{
	if (write(1, &c, 1) == -1)
		return (-1);
	(*count_chars)++;
	return (0);
}
