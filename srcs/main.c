#include "minibash.h"

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    while (1) {
        char* line = readline("minibash$ ");
        if (line == NULL)
            break;

        if (line[0] == '\0') {
            free(line);
            continue;
        }

        add_history(line);

        list_t* cmds_lst = parser(line);

        lst_clear(cmds_lst, free_cmd);
        free(line);
    }

    return 0;
}
