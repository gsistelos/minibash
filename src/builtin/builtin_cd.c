#include "minibash.h"

/*
 * @brief Builtin cd
 * @param cmd The command to execute
 * @return 1 if error, otherwise 0
 **/
int builtin_cd(cmd_t* cmd) {
    char* path = cmd->args[1];
    if (path == NULL) {
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "minibash: cd: HOME not set\n");
            return 1;
        }
    } else if (cmd->args[2] != NULL) {
        fprintf(stderr, "minibash: cd: too many arguments\n");
        return 1;
    }

    if (chdir(path) == -1) {
        perror("minibash: cd");
        return 1;
    }

    return 0;
}
