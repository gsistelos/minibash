#include "minibash.h"

void lst_clear(list_t* lst, void (*f)(void*))
{
	while (lst) {
		list_t* tmp = lst->next;
		f(lst->content);
		free(lst);
		lst = tmp;
	}
}
