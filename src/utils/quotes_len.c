#include "minibash.h"

/*
 * @brief Returns the length of the string between quotes
 * @param str The string to check
 * @return The length of the string between quotes, 0 if the string is not between quotes, -1 if the quotes are not closed
 **/
size_t quotes_len(char* str) {
    if (str[0] != '\'' && str[0] != '\"')
        return 0;

    char find = str[0];

    size_t len = strchrlen(++str, find);

    if (str[len] == '\0')
        return -1;

    return len + 1;
}
