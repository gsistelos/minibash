#include "minibash.h"

/*
 * @brief Parse a string into a list of commands
 * @param str The string to parse
 * @return A list of commands on success, NULL on failure
 **/
list_t* parser(char* str) {
    list_t* token_list = lexer(str);
    if (token_list == NULL) {
        g_status_code = 2;
        return NULL;
    }

    if (expansor(token_list) != 0) {
        list_clear(token_list, free_token);
        g_status_code = 1;
        return NULL;
    }

    g_status_code = 0;

    list_t* cmd_list = interpreter(token_list);
    list_clear(token_list, free_token);
    if (cmd_list == NULL) {
        g_status_code = 2;
        return NULL;
    }

    return cmd_list;
}
