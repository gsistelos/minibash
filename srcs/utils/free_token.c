#include "minibash.h"

void free_token(void* content) {
    if (content == NULL)
        return;

    token_t* token = content;

    free(token->str);
    free(token);
}
