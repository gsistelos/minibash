#include "minibash.h"

int set_pipes(list_t* cmds_lst, int* bridge_pipe) {
    cmd_t* cmd = cmds_lst->data;

    if (*bridge_pipe != -1) {
        if (cmd->input == STDIN_FILENO)
            cmd->input = *bridge_pipe;
        else
            close(*bridge_pipe);
    }

    if (cmds_lst->next == NULL) {
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
