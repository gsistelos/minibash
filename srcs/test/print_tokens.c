#include "minibash.h"

void print_tokens(list_t* tokens_lst) {
    while (tokens_lst) {
        token_t* token = tokens_lst->data;

        printf("%s | ", token->str);

        if (token->type == WORD)
            printf("WORD\n");
        else if (token->type == REDIR)
            printf("REDIR\n");
        else if (token->type == PIPE)
            printf("PIPE\n");

        tokens_lst = tokens_lst->next;
    }
}
