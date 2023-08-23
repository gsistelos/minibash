#include "minibash.h"

int builtin_test(cmd_t* cmd) {
    if (cmd->args[1] == NULL) {
        fprintf(stderr, "test: too few arguments\n");
        return 1;
    }

    printf("%s\n", cmd->args[1]);
    return 0;
}
