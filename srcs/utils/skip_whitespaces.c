#include "minibash.h"

/*
 * @brief Skips all whitespaces in the input string
 * @param str The input string
 * @return A pointer to the first non-whitespace character
 **/
char* skip_whitespaces(char* str) {
    while (isspace(*str))
        str++;
    return str;
}
