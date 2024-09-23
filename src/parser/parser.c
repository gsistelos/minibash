#include "minibash.h"

#include <stdlib.h>

/*
 * @brief Parse a string into a list of commands
 * @param str The string to parse
 * @return A list of commands on success, NULL on failure
 **/
t_lst* parser(char* str) {
    t_lst* token_list = lexer(str);
    if (token_list == NULL) {
        g_status_code = 2;
        return NULL;
    }

    if (expansor(token_list->head) != 0) {
        lst_del(token_list, free_token);
        g_status_code = 1;
        return NULL;
    }

    g_status_code = 0;

    t_lst* cmd_list = interpreter(token_list->head);
    lst_del(token_list, free_token);
    if (cmd_list == NULL) {
        g_status_code = 2;
        return NULL;
    }

    return cmd_list;
}
