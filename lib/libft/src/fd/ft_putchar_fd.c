#include "ft_fd.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
