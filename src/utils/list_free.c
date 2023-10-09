#include "minibash.h"

/*
 * @brief Free a node of the list
 * @param list The node to free
 * @param func The function to free the data
 * @note This function does not free all the list, use list_clear() for that
 **/
void list_free(list_t* list, void (*func)(void*)) {
    if (list == NULL)
        return;

    if (func != NULL)
        func(list->data);

    free(list);
}
