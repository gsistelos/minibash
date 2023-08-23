#include "minibash.h"

void wait_pids(pid_t* pid, size_t size) {
    int status;

    for (size_t i = 0; i < size; i++) {
        waitpid(pid[i], &status, 0);
        if (WIFEXITED(status))
            g_status_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_status_code = WTERMSIG(status) + 128;
    }
}
