#include "minibash.h"

/*
 * @brief Forks, dup2s and executes a command using exec_func
 * @param exec_func The function to execute
 * @param cmd The command to execute
 * @param bridge_pipe The pipe connected to the next cmd that needs to be closed
 * @return The child pid, 0 if the current process is the child, -1 if error
 **/
pid_t fork_exec(int (*exec_func)(cmd_t* cmd), cmd_t* cmd, int bridge_pipe) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("minibash: fork");
        return -1;
    }

    if (pid == 0) {
        if (bridge_pipe != -1)
            close(bridge_pipe);

        if (cmd->input != STDIN_FILENO) {
            dup2(cmd->input, STDIN_FILENO);
            close(cmd->input);
        }

        if (cmd->output != STDOUT_FILENO) {
            dup2(cmd->output, STDOUT_FILENO);
            close(cmd->output);
        }

        exec_func(cmd);
        return 0;
    }

    if (cmd->input != STDIN_FILENO)
        close(cmd->input);
    if (cmd->output != STDOUT_FILENO)
        close(cmd->output);

    return pid;
}
