#include "minibash.h"

/*
 * @brief Concatenate two strings into a new allocated string
 * @param s1 The first string
 * @param s2 The second string
 * @return The new allocated string, must be freed
 **/
char* strjoin(const char* s1, const char* s2) {
    size_t s1_len = s1 ? strlen(s1) : 0;
    size_t s2_len = s2 ? strlen(s2) : 0;

    char* new_str = malloc(sizeof(char) * (s1_len + s2_len + 1));
    if (new_str == NULL)
        return NULL;

    if (s1 != NULL)
        memcpy(new_str, s1, s1_len);
    if (s2 != NULL)
        memcpy(new_str + s1_len, s2, s2_len);

    new_str[s1_len + s2_len] = '\0';

    return new_str;
}
