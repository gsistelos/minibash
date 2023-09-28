#include "minibash.h"

char* get_path(char* cmd) {
    if (cmd[0] == '.' || cmd[0] == '/')
        return (strdup(cmd));

    char* path = getenv("PATH");
    if (!path) {
        fprintf(stderr, "minibash: %s: command not found\n", cmd);
        return (NULL);
    }

    char* aux = strjoin("/", cmd);
    if (!aux)
        return (NULL);

    char** path_arr = split(path, ':');
    if (!path_arr) {
        free(aux);
        perror("minibash: malloc");
        return (NULL);
    }

    for (size_t i = 0; path_arr[i]; i++) {
        char* tmp = strjoin(path_arr[i], aux);
        free(path_arr[i]);
        if (!tmp) {
            matrix_free((void**)path_arr);
            free(aux);
            perror("minibash: malloc");
            return (NULL);
        }

        path_arr[i] = tmp;

        if (!access(path_arr[i], F_OK)) {
            free(aux);
            aux = strdup(path_arr[i]);
            matrix_free((void**)path_arr);
            return (aux);
        }
    }

    matrix_free((void**)path_arr);
    free(aux);

    fprintf(stderr, "minibash: %s: command not found\n", cmd);
    return (NULL);
}
