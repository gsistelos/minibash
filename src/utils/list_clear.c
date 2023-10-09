#include "minibash.h"

/*
 * @brief Free a list
 * @param list The list to free
 * @param func The function to free the data
 **/
void list_clear(list_t* list, void (*func)(void*)) {
    while (list) {
        list_t* next = list->next;
        list_free(list, func);
        list = next;
    }
}
