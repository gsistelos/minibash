#include "minibash.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int set_pipes(t_node* cmd_node, int* bridge_pipe) {
    cmd_t* cmd = cmd_node->data;

    if (*bridge_pipe != -1) {
        if (cmd->input == STDIN_FILENO)
            cmd->input = *bridge_pipe;
        else
            close(*bridge_pipe);
    }

    if (cmd_node->next == NULL) {
        *bridge_pipe = -1;
        return 0;
    }

    int current_pipe[2];
    if (pipe(current_pipe) != 0) {
        perror("minibash: pipe");
        return -1;
    }

    if (cmd->output == STDOUT_FILENO)
        cmd->output = current_pipe[1];
    else
        close(current_pipe[1]);

    *bridge_pipe = current_pipe[0];

    return 0;
}

static pid_t run_multiple(t_node* cmd_node, size_t size) {
    int bridge_pipe = -1;

    pid_t* pid = malloc(sizeof(pid_t) * size);
    if (pid == NULL) {
        perror("minibash: malloc");
        return 1;
    }

    for (size_t i = 0; cmd_node; i++) {
        if (set_pipes(cmd_node, &bridge_pipe) != 0)
            return 1;

        cmd_t* cmd = cmd_node->data;

        int (*exec_func)(cmd_t*) = get_builtin(cmd->args[0]);
        if (exec_func == NULL)
            exec_func = execve_cmd;

        pid[i] = fork_exec(exec_func, cmd, bridge_pipe);
        if (pid[i] == 0) {
            free(pid);
            return 0;
        }

        cmd_node = cmd_node->next;
    }

    wait_pids(pid, size);
    free(pid);

    return 1;
}

/*
 * @brief Execute the commands in the cmd_list
 * @param cmd_list The list of commands
 * @return 0 if the current process is the child, 1 if it's the parent
 **/
pid_t executor(t_lst* cmd_list) {
    if (cmd_list->size == 1) {
        if (run_cmd(cmd_list->head->data) == 0)
            return 0;
    } else {
        if (run_multiple(cmd_list->head, cmd_list->size) == 0)
            return 0;
    }

    return 1;
}
