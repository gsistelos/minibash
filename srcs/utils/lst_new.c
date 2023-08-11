#include "minibash.h"

list_t* lst_new(void* content) {
    list_t* new = malloc(sizeof(list_t));
    if (new == NULL)
        return NULL;

    new->content = content;
    new->next = NULL;

    return new;
}
