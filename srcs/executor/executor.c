#include "minibash.h"

pid_t run_single(list_t* cmds_lst) {
    int (*builtin)(cmd_t*) = get_builtin(cmds_lst->data);
    if (builtin) {
        g_status_code = builtin(cmds_lst->data);
        return 1;
    }

    pid_t pid = fork_exec(execve_cmd, cmds_lst->data, -1);
    if (pid == 0)
        return 0;

    wait_pids(&pid, 1);
    return 1;
}

pid_t run_multiple(list_t* cmds_lst, size_t size) {
    int bridge_pipe = -1;

    pid_t* pid = malloc(sizeof(pid_t) * size);
    if (!pid) {
        perror("minibash: malloc");
        return 1;
    }

    size_t i = 0;
    while (cmds_lst) {
        if (set_pipes(cmds_lst, &bridge_pipe))
            return 1;

        int (*exec_func)(cmd_t*) = get_builtin(cmds_lst->data);
        if (exec_func == NULL)
            exec_func = execve_cmd;

        pid[i] = fork_exec(exec_func, cmds_lst->data, bridge_pipe);
        if (pid[i] == 0) {
            free(pid);
            return 0;
        }

        cmds_lst = cmds_lst->next;
        i++;
    }

    wait_pids(pid, size);
    free(pid);
    return 1;
}

/*
 * @brief Execute the commands in the cmd list
 * @param cmds_lst The list of commands
 * @return 0 if the current process is the child, 1 if it's the parent
 */
pid_t executor(list_t* cmds_lst) {
    size_t size = list_size(cmds_lst);

    if (size == 1) {
        if (run_single(cmds_lst) == 0)
            return 0;
    } else {
        if (run_multiple(cmds_lst, size) == 0)
            return 0;
    }

    return 1;
}