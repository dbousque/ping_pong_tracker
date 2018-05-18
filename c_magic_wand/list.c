

#include "ping_pong.h"

void	init_list(t_list *list, size_t elt_size)
{
	list->elts = NULL;
	list->size = 0;
	list->len = 0;
	list->elt_size = elt_size;
}

void	double_list_size(t_list *lst)
{
	char	*new_elts;
	char	*elts;
	int		i;
	size_t	x;

	lst->size *= 2;
	if (lst->size == 0)
		lst->size = 4;
	if (!(new_elts = (char*)malloc(lst->elt_size * lst->size)))
		return ;
	elts = (char*)lst->elts;
	i = 0;
	while (i < lst->len)
	{
		x = 0;
		while (x < lst->elt_size)
		{
			new_elts[i * lst->elt_size + x] = elts[i * lst->elt_size + x];
			x++;
		}
		i++;
	}
	free(lst->elts);
	lst->elts = (void*)new_elts;
}

void	*new_elt(t_list *lst)
{
	char	*elts;

	if (lst->size == lst->len)
		double_list_size(lst);
	if (!lst->elts)
		return (NULL);
	lst->len++;
	elts = (char*)lst->elts;
	return (&(elts[lst->elt_size * (lst->len - 1)]));
}

void	remove_elt(t_list *lst, char *addr)
{
	char	*elts;

	elts = lst->elts;
	addr += lst->elt_size;
	while (addr < elts + (lst->len * lst->elt_size))
	{
		if (addr - lst->elt_size >= elts)
			*(addr - lst->elt_size) = *addr;
		addr++;
	}
	lst->len--;
}

void	*pop_elt(t_list *lst)
{
	char	*elts;

	if (lst->len <= 0)
		return (NULL);
	elts = (char*)lst->elts;
	lst->len--;
	return (&(elts[lst->elt_size * (lst->len)]));
}

void	free_list(t_list *lst)
{
	if (lst->elts)
		free(lst->elts);
}
