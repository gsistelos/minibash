#include "minibash.h"

/*
 * @brief Get the last node of a list
 * @param list The list
 * @return The last node of the list
 **/
list_t* list_last(list_t* list) {
    if (list == NULL)
        return NULL;

    while (list->next)
        list = list->next;

    return list;
}
