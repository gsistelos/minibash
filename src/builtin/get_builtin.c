#include "minibash.h"

/*
 * @brief Get a pointer to a builtin function
 * @param cmd The builtin command to get
 * @return A pointer to the builtin function or NULL if not a builtin
 **/
void* get_builtin(cmd_t* cmd) {
    if (cmd->args[0] == NULL)
        return &builtin_null;

    if (strcmp(cmd->args[0], "env") == 0)
        return &builtin_env;
    if (strcmp(cmd->args[0], "export") == 0)
        return &builtin_export;
    if (strcmp(cmd->args[0], "exit") == 0)
        return &builtin_exit;
    if (strcmp(cmd->args[0], "test") == 0)
        return &builtin_test;
    if (strcmp(cmd->args[0], "unset") == 0)
        return &builtin_unset;

    return NULL;
}
