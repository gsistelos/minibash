#include "minibash.h"

size_t quotes_len(char* str) {
    if (str[0] != '\'' && str[0] != '\"')
        return 0;

    char find = str[0];

    size_t len = 1;

    while (str[len] && str[len] != find)
        len++;

    if (str[len] == '\0')
        return -1;

    return len;
}
