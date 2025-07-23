#include "ft_function.h"
#include "ft_string.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*ptr;
	size_t	size;
	size_t	i;

	size = ft_strlen((const char *)s);
	ptr = malloc(sizeof(char) * (size + 1));
	if (ptr == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (i < size)
	{
		ptr[i] = f(i, s[i]);
		i ++;
	}
	ptr[i] = '\0';
	return (ptr);
}
