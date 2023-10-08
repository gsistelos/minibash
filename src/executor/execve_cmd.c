#include "minibash.h"

/*
 * @brief Executes a command using execve
 * @param cmd The command to execute
 * @return 1 if error, otherwise the process will be replaced by the command
 **/
int execve_cmd(cmd_t* cmd) {
    char* path = get_path(cmd->args[0]);
    if (errno != 0)
        return 1;

    if (path == NULL)
        return 1;

    execve(path, cmd->args, environ);
    perror("minibash: execve");
    return 1;
}
