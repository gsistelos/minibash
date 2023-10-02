#include "minibash.h"

/* @brief Parse a string into a list of commands
 * @param str The string to parse
 * @return A list of commands on success, NULL on failure
 **/
list_t* parser(char* str) {
    list_t* tokens_lst = lexer(str);
    if (tokens_lst == NULL) {
        g_status_code = 2;
        return NULL;
    }

    if (expansor(tokens_lst) != 0) {
        list_clear(tokens_lst, free_token);
        g_status_code = 1;
        return NULL;
    }

    list_t* cmds_lst = interpreter(tokens_lst);
    list_clear(tokens_lst, free_token);
    if (cmds_lst == NULL) {
        g_status_code = 2;
        return NULL;
    }

    return cmds_lst;
}
