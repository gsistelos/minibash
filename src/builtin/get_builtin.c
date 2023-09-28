#include "minibash.h"

/*
 * @brief Get a pointer to a builtin function
 * @param cmd The builtin command to get
 * @return A pointer to the builtin function or NULL if not a builtin
 **/
void* get_builtin(cmd_t* cmd) {
    if (!strcmp(cmd->args[0], "exit"))
        return &builtin_exit;
    else if (!strcmp(cmd->args[0], "test"))
        return &builtin_test;

    return NULL;
}
