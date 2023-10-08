#include "minibash.h"

int g_status_code = 0;

int main(void) {
    if (setup_signals())
        return 1;

    while (1) {
        errno = 0;

        char* line = readline("minibash$ ");
        if (errno != 0) {
            perror("minibash: readline");
            continue;
        }

        if (line == NULL)
            break;

        char* line_start = skip_whitespaces(line);
        if (*line_start == '\0') {
            free(line);
            continue;
        }

        add_history(line);
        if (errno != 0)
            perror("minibash: add_history");

        list_t* cmd_list = parser(line_start);
        free(line);
        if (cmd_list == NULL)
            continue;

        pid_t pid = executor(cmd_list);
        list_clear(cmd_list, free_cmd);
        if (pid == 0)
            return 1;
    }

    return 0;
}
