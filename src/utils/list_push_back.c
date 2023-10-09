#include "minibash.h"

/*
 * @brief Push a node at the end of a list
 * @param list The list
 * @param new The node to push
 * @return 0 in case of success, 1 if the node was not added
 **/
int list_push_back(list_t** list, list_t* new) {
    if (new == NULL)
        return 1;

    list_t* last = list_last(*list);

    if (last == NULL)
        *list = new;
    else
        last->next = new;

    return 0;
}
