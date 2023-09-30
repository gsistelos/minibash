#include "minibash.h"

static int apply_expansion(char** new_str, char* aux) {
    if (aux) {
        char* tmp = strjoin(*new_str, aux);
        free(aux);
        if (!tmp)
            return 1;

        free(*new_str);
        *new_str = tmp;
    }

    return 0;
}

static size_t no_expansion(char* str, char** buf) {
    size_t len = 0;
    while (!strchr("$~'\"", str[len]))
        ++len;

    *buf = strndup(str, len);
    if (!*buf)
        return -1;

    return len;
}

static size_t env_expansion(char* str, char** buf) {
    ++str;

    size_t len = 0;
    while (isalnum(str[len]) || str[len] == '_')
        ++len;

    char* aux = strndup(str, len);
    if (!aux)
        return -1;

    char* env = getenv(aux);
    free(aux);

    if (env)
        *buf = strdup(env);
    else
        *buf = NULL;

    return len + 1;
}

static size_t name_expansion(char** buf, char* name, size_t len) {
    char* env = getenv(name);

    if (env)
        *buf = strdup(env);
    else
        *buf = NULL;

    return len;
}

static size_t double_quotes_expansion(char* str, char** buf) {
    ++str;

    size_t total_len = 0;

    char* new_str = NULL;

    while (*str != '"') {
        char* aux;
        size_t len;

        if (*str == '$')
            len = env_expansion(str, &aux);
        else if (*str == '~')
            len = name_expansion(&aux, "HOME", 1);
        else if (*str == '\'') {
            len = 1;
            aux = strdup("'");
        } else
            len = no_expansion(str, &aux);

        if (errno != 0) {
            perror("minibash: malloc");
            return -1;
        }

        if (apply_expansion(&new_str, aux))
            return -1;

        str += len;
        total_len += len;
    }

    *buf = new_str;

    return total_len + 2;
}

static size_t single_quotes_expansion(char* str, char** buf) {
    ++str;

    size_t len = 0;
    while (str[len] != '\'')
        ++len;

    char* new_str = strndup(str, len);
    if (!new_str)
        return -1;

    *buf = new_str;

    return len + 2;
}

char* expand(char* str) {
    char* new_str = NULL;

    while (*str) {
        char* aux;
        size_t len;

        if (*str == '$')
            len = env_expansion(str, &aux);
        else if (*str == '~')
            len = name_expansion(&aux, "HOME", 1);
        else if (*str == '"')
            len = double_quotes_expansion(str, &aux);
        else if (*str == '\'')
            len = single_quotes_expansion(str, &aux);
        else
            len = no_expansion(str, &aux);

        if (errno != 0) {
            perror("minibash: malloc");
            return NULL;
        }

        if (apply_expansion(&new_str, aux))
            return NULL;

        str += len;
    }

    return new_str;
}
