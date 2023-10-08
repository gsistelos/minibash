#include "minibash.h"

/*
 * @brief Get a pointer to a builtin function
 * @param cmd The command name
 * @return A pointer to the builtin function or NULL if it is not a builtin
 **/
void* get_builtin(char* cmd) {
    if (cmd == NULL)
        return &builtin_null;

    if (strcmp(cmd, "cd") == 0)
        return &builtin_cd;
    if (strcmp(cmd, "env") == 0)
        return &builtin_env;
    if (strcmp(cmd, "exit") == 0)
        return &builtin_exit;
    if (strcmp(cmd, "export") == 0)
        return &builtin_export;
    if (strcmp(cmd, "unset") == 0)
        return &builtin_unset;

    return NULL;
}
