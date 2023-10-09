#include "minibash.h"

/*
 * @brief Create a new node of the list
 * @param data The data to store in the node
 * @return The new node
 * @note The node must be freed with list_free()
 **/
list_t* list_new(void* data) {
    list_t* list = malloc(sizeof(list_t));
    if (list == NULL)
        return NULL;

    list->data = data;
    list->next = NULL;

    return list;
}
