#include "minibash.h"

/*
 * @brief Run a command
 * @param cmd_list The command
 * @return 0 if the process is a child, 1 otherwise
 **/
pid_t run_cmd(cmd_t* cmd) {
    int (*builtin)(cmd_t*) = get_builtin(cmd->args[0]);
    if (builtin != NULL) {
        g_status_code = builtin(cmd);
        return 1;
    }

    pid_t pid = fork_exec(execve_cmd, cmd, -1);
    if (pid == -1)
        return 1;

    if (pid == 0)
        return 0;

    wait_pids(&pid, 1);
    return 1;
}
