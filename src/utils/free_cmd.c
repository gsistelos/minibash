#include "minibash.h"

/*
 * @brief Frees a command
 * @param data The command to free
 * @note Used in list_free
 **/
void free_cmd(void* data) {
    if (data == NULL)
        return;

    cmd_t* cmd = data;

    matrix_free((void**)cmd->args);
    free(cmd);
}
