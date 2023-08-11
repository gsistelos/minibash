#include "minibash.h"

list_t* parser(char* str) {
    list_t* tokens_lst = lexer(str);
    if (tokens_lst == NULL)
        return NULL;

    if (expansor(tokens_lst)) {
        lst_clear(tokens_lst, free_token);
        return NULL;
    }

    print_tokens(tokens_lst);

    list_t* cmds_lst = interpreter(tokens_lst);
    lst_clear(tokens_lst, free_token);

    return cmds_lst;
}
