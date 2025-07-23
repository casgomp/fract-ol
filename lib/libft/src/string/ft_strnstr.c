#include "ft_string.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (little[i] == '\0')
	{
		return ((char *)big);
	}
	while (big[i] && (i < len))
	{
		if (big[i] == little[0])
		{
			j = 0;
			while (little[j] && ((big[i + j] == little[j])) && (i + j < len))
			{
				j ++;
			}
			if (j == ft_strlen(little))
			{
				return ((char *)(big + i));
			}
		}
		i ++;
	}
	return (NULL);
}
