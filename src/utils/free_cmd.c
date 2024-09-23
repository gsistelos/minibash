#include "minibash.h"

#include <stdlib.h>

/*
 * @brief Frees a command
 * @param ptr The command to free
 * @note Used in list_free
 **/
void free_cmd(void* ptr) {
    if (ptr == NULL)
        return;

    cmd_t* cmd = ptr;

    gs_matrix_free((void**)cmd->args);
    free(cmd);
}
