#include "minibash.h"

cmd_t* new_cmd(char** args, int input, int output) {
    if (args == NULL)
        return NULL;

    cmd_t* cmd = malloc(sizeof(cmd_t));
    if (cmd == NULL)
        return NULL;

    cmd->args = args;
    cmd->input = input;
    cmd->output = output;

    return cmd;
}
