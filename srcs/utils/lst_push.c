#include "minibash.h"

int lst_push(list_t** lst, list_t* new)
{
	if (new == NULL)
		return 1;
	if ((*lst) == NULL) {
		*lst = new;
		return 0;
	}
	lst_last(*lst)->next = new;
	return 0;
}
