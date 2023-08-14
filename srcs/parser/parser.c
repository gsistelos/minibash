#include "minibash.h"

list_t* parser(char* str) {
    list_t* tokens_lst = lexer(str);
    if (tokens_lst == NULL) {
        g_status_code = 2;
        return NULL;
    }

    if (expansor(tokens_lst)) {
        list_clear(tokens_lst, free_token);
        g_status_code = 1;
        return NULL;
    }

    list_t* cmds_lst = interpreter(tokens_lst);
    list_clear(tokens_lst, free_token);

    return cmds_lst;
}
