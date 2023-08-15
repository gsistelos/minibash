#include "minibash.h"

extern char** environ;

/*
 * @brief Execute the first command in the cmd list
 * @param cmd_list The list of commands
 * @param prev_pipe The previous pipe to read from
 * @param pipe The current pipe
 * @return 0 if the current process is the child, -1 if an error occured, otherwise the pid of the child
 **/
static pid_t exec_cmd(list_t* cmd_list, int* prev_pipe, int pipefd[2]) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("minibash: fork");
        return -1;
    }

    cmd_t* cmd = cmd_list->data;

    if (pid == 0) {
        close(pipefd[0]);

        if (cmd->input != STDIN_FILENO) {
            dup2(cmd->input, STDIN_FILENO);
            close(cmd->input);
        } else if (*prev_pipe != -1) {
            dup2(*prev_pipe, STDIN_FILENO);
            close(*prev_pipe);
        }

        if (cmd->output != STDOUT_FILENO) {
            dup2(cmd->output, STDOUT_FILENO);
            close(cmd->output);
        } else if (cmd_list->next != NULL) {
            dup2(pipefd[1], STDOUT_FILENO);
        }

        close(pipefd[1]);

        char* path = get_path(cmd->args[0]);
        if (path == NULL)
            return 0;

        execve(path, cmd->args, environ);
        perror("minibash: execve");
        return 0;
    }

    close(pipefd[1]);

    if (*prev_pipe != -1)
        close(*prev_pipe);

    if (cmd_list->next != NULL)
        *prev_pipe = pipefd[0];

    if (cmd->input != STDIN_FILENO)
        close(cmd->input);
    if (cmd->output != STDOUT_FILENO)
        close(cmd->output);

    return pid;
}

/*
 * @brief Execute the commands in the cmd list
 * @param cmds_lst The list of commands
 * @return 0 if the current process is the child, 1 if it's the parent
 */
pid_t executor(list_t* cmd_list) {
    int prev_pipe = -1;
    size_t size = list_size(cmd_list);

    pid_t* pid = malloc(sizeof(pid_t) * size);

    size_t i = 0;
    while (cmd_list) {
        int pipefd[2];
        pipe(pipefd);

        pid[i] = exec_cmd(cmd_list, &prev_pipe, pipefd);
        if (pid[i] == 0) {
            free(pid);
            return 0;
        }

        cmd_list = cmd_list->next;
        i++;
    }

    int status;

    for (size_t i = 0; i < size; i++) {
        waitpid(pid[i], &status, 0);
        if (WIFEXITED(status))
            g_status_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_status_code = WTERMSIG(status) + 128;
    }

    free(pid);

    return 1;
}