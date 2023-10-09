#include "minibash.h"

/*
 * @brief Get the size of a list
 * @param list The list to get the size of
 * @return The size of the list
 **/
size_t list_size(list_t* list) {
    size_t size = 0;

    while (list) {
        size++;
        list = list->next;
    }

    return size;
}
