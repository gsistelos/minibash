#include "minibash.h"

list_t* lst_last(list_t* lst)
{
	while (lst->next)
		lst = lst->next;
	return lst;
}
