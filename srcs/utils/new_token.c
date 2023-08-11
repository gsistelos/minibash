#include "minibash.h"

token_t* new_token(char* str) {
    if (str == NULL)
        return NULL;

    token_t* token = malloc(sizeof(token_t));
    if (token == NULL)
        return NULL;

    token->str = str;

    if (str[0] == '|')
        token->type = PIPE;
    else if (strchr("><", str[0]))
        token->type = REDIR;
    else
        token->type = WORD;

    return token;
}
