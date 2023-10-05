#include "minibash.h"

/*
 * @brief Unset a variable
 * @param cmd The command to execute
 * @return 0 on success, 1 on error
 **/
int builtin_unset(cmd_t* cmd) {
    for (size_t i = 1; cmd->args[i]; i++)
        unsetenv(cmd->args[i]);

    return 0;
}
