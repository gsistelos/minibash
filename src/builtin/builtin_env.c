#include "minibash.h"

/*
 * @brief Prints the environment variables
 * @param cmd The command to execute
 * @return 0 on success, 1 on error
 **/
int builtin_env(cmd_t *cmd) {
    for (size_t i = 0; environ[i]; i++) {
        write(cmd->output, environ[i], strlen(environ[i]));
        write(cmd->output, "\n", 1);
    }

    return 0;
}
