#include "minibash.h"

/*
 * @brief Creates a new command
 * @param args The arguments of the command
 * @param input The input file descriptor
 * @param output The output file descriptor
 * @return The new command, NULL if an error occured
 **/
cmd_t* new_cmd(char** args, int input, int output) {
    cmd_t* cmd = malloc(sizeof(cmd_t));
    if (cmd == NULL)
        return NULL;

    cmd->args = args;
    cmd->input = input;
    cmd->output = output;

    return cmd;
}
