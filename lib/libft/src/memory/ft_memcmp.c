#include "ft_memory.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*x;
	unsigned char	*y;	

	x = (unsigned char *)s1;
	y = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (x[i] != y[i])
		{
			return (x[i] - y[i]);
		}
		i ++;
	}
	return (0);
}
