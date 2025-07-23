#include "ft_linked_list.h"

int	ft_lstsize(t_list *lst)
{
	int		i;
	t_list	*current;

	current = lst;
	i = 0;
	while (current != NULL)
	{
		i ++;
		current = current->next;
	}
	return (i);
}
