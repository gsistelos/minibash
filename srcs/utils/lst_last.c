#include "minibash.h"

list_t* lst_last(list_t* lst) {
    if (lst == NULL)
        return NULL;

    while (lst->next)
        lst = lst->next;

    return lst;
}
