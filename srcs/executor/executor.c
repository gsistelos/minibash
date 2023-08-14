#include "minibash.h"

extern char** environ;

static int setup_pipes(list_t* cmds_lst, size_t size) {
    int fd[2];
    int prev_fd = STDIN_FILENO;

    for (size_t i = 0; i < size - 1; i++) {
        if (pipe(fd) == -1) {
            perror("minibash: pipe");
            return 1;
        }

        cmd_t* cmd = cmds_lst->data;

        if (cmd->input == STDIN_FILENO)
            cmd->input = prev_fd;
        if (cmd->output == STDOUT_FILENO)
            cmd->output = fd[1];

        prev_fd = fd[0];

        cmds_lst = cmds_lst->next;
    }

    cmd_t* cmd = cmds_lst->data;

    if (cmd->input == STDIN_FILENO)
        cmd->input = prev_fd;

    return 0;
}

static int call_dup2(int fd, int fd2) {
    if (fd != fd2) {
        if (dup2(fd, fd2) == -1) {
            perror("minibash: dup2");
            return 1;
        }
        close(fd);
    }
    return 0;
}

/*
 * @brief Execute the commands in the cmd list
 * @param cmds_lst The list of commands
 * @return 1 if the current process is the child, 0 if it's the parent
 */
int executor(list_t* cmds_lst) {
    size_t size = list_size(cmds_lst);

    if (setup_pipes(cmds_lst, size))
        return 0;

    pid_t* pids = malloc(sizeof(pid_t) * size);
    if (pids == NULL) {
        perror("minibash: malloc");
        return 0;
    }

    for (size_t i = 0; i < size; i++) {
        cmd_t* cmd = cmds_lst->data;

        pids[i] = fork();
        if (pids[i] == -1) {
            perror("minibash: fork");
            return 0;
        }

        if (pids[i] == 0) {
            call_dup2(cmd->input, STDIN_FILENO);
            call_dup2(cmd->output, STDOUT_FILENO);
            signal(SIGQUIT, SIG_DFL);

            list_clear(cmds_lst->next, free_cmd);

            char* path = get_cmd_path(cmd->args[0]);
            if (path == NULL)
                return 1;

            if (execve(path, cmd->args, environ) == -1) {
                perror("minibash: execve");
                return 1;
            }
        }

        safe_close(cmd->input, cmd->output);

        cmds_lst = cmds_lst->next;
    }

    int status;

    for (size_t i = 0; i < size; i++) {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
            g_status_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_status_code = WTERMSIG(status) + 128;
    }

    return 0;
}
