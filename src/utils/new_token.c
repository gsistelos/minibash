#include "minibash.h"

/*
 * @brief Creates a new token
 * @param str The string to create the token from
 * @return The new token, NULL if an error occured
 * @note Token type is set according to the str
 **/
token_t* new_token(char* str) {
    token_t* token = malloc(sizeof(token_t));
    if (token == NULL)
        return NULL;

    token->str = str;

    if (*str == '|')
        token->type = PIPE;
    else if (strchr("><", *str) != NULL)
        token->type = REDIR;
    else
        token->type = WORD;

    return token;
}
