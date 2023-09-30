#include "minibash.h"

/*
 * @brief Expand all the tokens in the list and check for ambigous redirects
 * @param tokens_lst The list of tokens
 * @return 0 on success, 1 on error
 */
int expansor(list_t* tokens_lst) {
    int prev_type = PIPE;

    while (tokens_lst) {
        token_t* token = tokens_lst->data;

        if (token->type != WORD) {
            tokens_lst = tokens_lst->next;
            continue;
        }

        char* new_str = expand(token->str);
        if (errno != 0) {
            perror("minibash: malloc");
            return 1;
        }

        if (new_str == NULL && prev_type == REDIR) {
            fprintf(stderr, "minibash: %s: ambigous redirect\n", token->str);
            return 1;
        }

        prev_type = token->type;

        free(token->str);
        token->str = new_str;

        tokens_lst = tokens_lst->next;
    }

    return 0;
}
