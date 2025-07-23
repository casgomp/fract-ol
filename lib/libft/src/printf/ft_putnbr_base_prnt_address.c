#include "ft_printf.h"
#include "ft_string.h"

int	ft_putnbr_base_prnt_address(uintptr_t nb, int *count_chars, char *base)
{
	if ((size_t)nb >= ft_strlen(base))
	{
		ft_putnbr_base_prnt_address(nb / ft_strlen(base), count_chars, base);
		ft_putnbr_base_prnt_address(nb % ft_strlen(base), count_chars, base);
	}
	if ((size_t)nb < ft_strlen(base))
	{
		if (ft_putchar_prnt((int)base[nb], count_chars) == -1)
			return (-1);
	}
	return (0);
}
