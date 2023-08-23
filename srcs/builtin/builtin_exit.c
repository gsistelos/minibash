#include "minibash.h"

int builtin_exit(cmd_t* cmd) {
    if (cmd->args[1] == NULL)
        exit(0);
    exit(atoll(cmd->args[1]));
    return 0;
}
