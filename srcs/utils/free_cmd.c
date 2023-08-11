#include "minibash.h"

void free_cmd(void* content) {
    if (content == NULL)
        return;

    cmd_t* cmd = content;

    for (size_t i = 0; cmd->args[i]; i++) {
        free(cmd->args[i]);
    }

    free(cmd->args);
    free(cmd);
}
