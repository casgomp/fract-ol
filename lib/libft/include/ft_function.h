#ifndef FT_FUNCTION_H
# define FT_FUNCTION_H

# include <stdlib.h> 

void	ft_striteri(char *s, void (*f)(unsigned int, char *));
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));

#endif
