#include "minibash.h"

int g_status_code;

int main(void) {
    if (setup_signals())
        return 1;

    while (1) {
        char* line = readline("minibash$ ");
        if (line == NULL)
            break;

        char* line_start = skip_whitespaces(line);
        if (line_start[0] == '\0') {
            free(line);
            continue;
        }

        add_history(line);

        list_t* cmds_lst = parser(line_start);
        free(line);
        if (cmds_lst == NULL)
            continue;

        int is_child = executor(cmds_lst);
        list_clear(cmds_lst, free_cmd);
        if (is_child)
            return 1;
    }

    return 0;
}
