#include "minibash.h"

static int apply_expansion(char** new_str, char* aux) {
    if (aux != NULL) {
        char* tmp = strjoin(*new_str, aux);
        free(aux);
        if (tmp == NULL) {
            perror("minibash: malloc");
            return 1;
        }

        free(*new_str);
        *new_str = tmp;
    }

    return 0;
}

static ssize_t no_expansion(char* str, char** buf) {
    ssize_t len = 0;
    while (!strchr("$~'\"", str[len]))
        len++;

    *buf = strndup(str, len);
    if (*buf == NULL) {
        perror("minibash: malloc");
        return -1;
    }

    return len;
}

static ssize_t question_mark_expansion(char** buf) {
    char status_str[4];

    sprintf(status_str, "%d", g_status_code);

    char* aux = strdup(status_str);
    if (aux == NULL) {
        perror("minibash: malloc");
        return -1;
    }

    *buf = aux;

    return 2;
}

static ssize_t env_expansion(char* str, char** buf) {
    str++;

    if (*str == '?')
        return question_mark_expansion(buf);

    ssize_t len = 0;
    while (isalnum(str[len]) || str[len] == '_')
        len++;

    char* aux = strndup(str, len);
    if (aux == NULL) {
        perror("minibash: malloc");
        return -1;
    }

    char* env = getenv(aux);
    free(aux);

    if (env == NULL)
        *buf = NULL;
    else {
        *buf = strdup(env);
        if (*buf == NULL) {
            perror("minibash: malloc");
            return -1;
        }
    }

    return len + 1;
}

static ssize_t name_expansion(char** buf, char* name, ssize_t len) {
    char* env = getenv(name);

    if (env == NULL)
        *buf = NULL;
    else {
        *buf = strdup(env);
        if (*buf == NULL) {
            perror("minibash: malloc");
            return -1;
        }
    }

    return len;
}

static ssize_t double_quotes_expansion(char* str, char** buf) {
    str++;

    ssize_t total_len = 0;

    char* new_str = strdup("");
    if (new_str == NULL) {
        perror("minibash: malloc");
        return -1;
    }

    while (*str != '"') {
        char* aux = NULL;
        ssize_t len;

        if (*str == '$')
            len = env_expansion(str, &aux);
        else if (*str == '~')
            len = name_expansion(&aux, "HOME", 1);
        else if (*str == '\'') {
            len = 1;
            aux = strdup("'");
        } else
            len = no_expansion(str, &aux);

        if (len == -1)
            return -1;

        if (apply_expansion(&new_str, aux) != 0)
            return -1;

        str += len;
        total_len += len;
    }

    *buf = new_str;

    return total_len + 2;
}

static ssize_t single_quotes_expansion(char* str, char** buf) {
    str++;

    ssize_t len = 0;
    while (str[len] != '\'')
        len++;

    char* new_str = strndup(str, len);
    if (new_str == NULL) {
        perror("minibash: malloc");
        return -1;
    }

    *buf = new_str;

    return len + 2;
}

/*
 * @brief Expand a string
 * @param str The string to expand
 * @return The expanded string on success, can be NULL, errno is set on error
 **/
char* expand(char* str) {
    char* new_str = NULL;

    while (*str) {
        char* aux = NULL;
        ssize_t len;

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

        if (len == -1)
            return NULL;

        if (apply_expansion(&new_str, aux) != 0)
            return NULL;

        str += len;
    }

    return new_str;
}
