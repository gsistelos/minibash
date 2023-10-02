#include "minibash.h"

/*
 * @brief Returns the length of the string between quotes
 * @param str The string to check
 * @return The length of the string between quotes, 0 if the string is not between quotes, -1 if the quotes are not closed
 **/
ssize_t quotes_len(char* str) {
    if (*str != '\'' && *str != '\"')
        return 0;

    char find = *str;

    ssize_t len = 1;
    while (str[len] && str[len] != find)
        len++;

    if (str[len] != find)
        return -1;

    return len;
}
