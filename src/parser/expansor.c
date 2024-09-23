#include "minibash.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static int ambigous_redirect(char* str) {
    fprintf(stderr, "minibash: %s: ambigous redirect\n", str);
    return 1;
}

/*
 * @brief Expand all the tokens->str in the list
 * @param token_node Head of the list of tokens
 * @return 0 on success, 1 on error
 **/
int expansor(t_node* token_node) {
    int prev_type = PIPE;

    while (token_node) {
        token_t* token = token_node->data;

        if (token->type == WORD) {
            char* new_str = expand(token->str);
            if (errno != 0)
                return 1;

            if (new_str == NULL && prev_type == REDIR)
                return ambigous_redirect(token->str);

            free(token->str);
            token->str = new_str;
        }

        prev_type = token->type;
        token_node = token_node->next;
    }

    return 0;
}
