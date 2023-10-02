#include "minibash.h"

/*
 * @brief Expand all the tokens->str in the list
 * @param tokens_lst The list of tokens
 * @return 0 on success, 1 on error
 **/
int expansor(list_t* tokens_lst) {
    int prev_type = PIPE;

    while (tokens_lst) {
        token_t* token = tokens_lst->data;

        if (token->type == WORD) {
            char* new_str = expand(token->str);
            if (errno != 0)
                return 1;

            if (new_str == NULL && prev_type == REDIR) {
                fprintf(stderr, "minibash: %s: ambigous redirect\n", token->str);
                return 1;
            }

            free(token->str);
            token->str = new_str;
        }

        prev_type = token->type;
        tokens_lst = tokens_lst->next;
    }

    return 0;
}
