#include "minibash.h"

int validate_path_verbose(char* path) {
    struct stat st;

    if (stat(path, &st) != 0) {
        fprintf(stderr, "minibash: %s: %s\n", path, strerror(errno));
        return 1;
    }

    if (S_ISDIR(st.st_mode) != 0) {
        fprintf(stderr, "minibash: %s: Is a directory\n", path);
        return 1;
    }

    return 0;
}

int validate_path(char* path) {
    struct stat st;

    if (stat(path, &st) != 0)
        return 1;

    if (S_ISDIR(st.st_mode) != 0)
        return 1;

    return 0;
}

char* get_path(char* cmd) {
    if (*cmd == '.' || *cmd == '/') {
        if (validate_path_verbose(cmd) != 0)
            return NULL;

        char* aux = strdup(cmd);
        if (aux == NULL) {
            perror("minibash: malloc");
            return NULL;
        }

        return aux;
    }

    char* path = getenv("PATH");
    if (path == NULL) {
        fprintf(stderr, "minibash: %s: command not found\n", cmd);
        return NULL;
    }

    char* aux = strjoin("/", cmd);
    if (aux == NULL) {
        perror("minibash: malloc");
        return NULL;
    }

    char** path_arr = split(path, ':');
    if (path_arr == NULL) {
        free(aux);
        perror("minibash: malloc");
        return NULL;
    }

    for (size_t i = 0; path_arr[i]; i++) {
        char* tmp = strjoin(path_arr[i], aux);
        free(path_arr[i]);
        if (tmp == NULL) {
            matrix_free((void**)path_arr);
            free(aux);
            return NULL;
        }

        path_arr[i] = tmp;

        if (validate_path(path_arr[i]) == 0) {
            free(aux);
            aux = strdup(path_arr[i]);
            matrix_free((void**)path_arr);
            errno = 0;
            return aux;
        }
    }

    matrix_free((void**)path_arr);
    free(aux);

    fprintf(stderr, "minibash: %s: command not found\n", cmd);
    return NULL;
}
