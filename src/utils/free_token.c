#include "minibash.h"

/*
 * @brief Frees a token
 * @param ptr The token to free
 * @note Used in list_free
 **/
void free_token(void* ptr) {
    if (ptr == NULL)
        return;

    token_t* token = ptr;

    free(token->str);
    free(token);
}
