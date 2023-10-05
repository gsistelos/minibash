#include "minibash.h"

/*
 * @brief Export a variable
 * @param cmd The command to execute
 * @return 0 on success, 1 on error
 **/
int builtin_export(cmd_t* cmd) {
    if (cmd->args[1] == NULL) {
        builtin_env(cmd);
        return 0;
    }

    for (size_t i = 1; cmd->args[i]; i++) {
        char* value = strchr(cmd->args[i], '=');
        if (value == NULL)
            return 0;

        *value = '\0';
        value++;

        if (setenv(cmd->args[i], value, 1) == -1) {
            perror("minibash: export");
            return 1;
        }
    }

    return 0;
}
