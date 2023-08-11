#include "minibash.h"

static size_t token_len(char* str) {
    if (!strncmp(str, ">>", 2) || !strncmp(str, "<<", 2))
        return 2;
    else if (strchr("|><", *str))
        return 1;

    size_t len = 0;
    while (!isspace(str[len]) && !strchr("|><", str[len])) {
        size_t quote_end = quotes_len(str + len);
        if (quote_end == (size_t)-1)
            return -1;

        len += quote_end + 1;
    }

    return len;
}

list_t* lexer(char* str) {
    list_t* tokens_lst = NULL;

    while (isspace(*str))
        str++;

    while (*str) {
        size_t len = token_len(str);
        if (len == (size_t)-1) {
            fprintf(stderr, "minibash: syntax error: unclosed quotes\n");
            lst_clear(tokens_lst, free_token);
            return NULL;
        }

        token_t* token = new_token(strndup(str, len));
        if (token == NULL) {
            lst_clear(tokens_lst, free_token);
            return NULL;
        }

        if (lst_push(&tokens_lst, lst_new(token))) {
            free_token(token);
            lst_clear(tokens_lst, free_token);
            return NULL;
        }

        str += len;

        while (isspace(*str))
            str++;
    }

    return tokens_lst;
}
