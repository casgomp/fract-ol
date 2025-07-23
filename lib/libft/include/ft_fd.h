#ifndef FT_FD_H
# define FT_FD_H

# include <unistd.h>
# include <stdio.h>

void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);

#endif
