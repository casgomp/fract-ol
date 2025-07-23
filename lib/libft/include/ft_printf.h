#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

int	ft_printf(const char *format, ...);
int	ft_putchar_prnt(int c, int *count_chars);
int	ft_putstr_prnt(void *str, int *count_chars);
int	ft_putnbr_base_prnt(int nb, int *count_chars, char *base);
int	ft_putnbr_base_prnt_address(uintptr_t nb, int *count_chars, char *base);
int	ft_putnbr_base_prnt_address_pre(uintptr_t nb, int 
		*count_chars, int prefix, int first_call);

#endif
