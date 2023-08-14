#include "minibash.h"

/*
 * @brief Frees a token
 * @param data The token to free
 * @note Used in list_free
 **/
void free_token(void* data) {
    if (data == NULL)
        return;

    token_t* token = data;

    free(token->str);
    free(token);
}
