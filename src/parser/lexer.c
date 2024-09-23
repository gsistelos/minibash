#include "liblst.h"
#include "minibash.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static t_lst* syntax_error(char* str) {
    fprintf(stderr, "minibash: syntax error near unexpected token '%s'\n", str);
    return NULL;
}

/*
 * @brief Get the length of the next token in a string
 * @param str The string to get the token length from
 * @return The length of the token, -1 on failure
 **/
static ssize_t token_len(char* str) {
    if (strncmp(str, ">>", 2) == 0 || strncmp(str, "<<", 2) == 0)
        return 2;
    if (strchr("|><", *str) != NULL)
        return 1;

    ssize_t len = 0;
    while (!isspace(str[len]) && !strchr("|><", str[len])) {
        ssize_t quote_end = quotes_len(str + len);
        if (quote_end == -1) {
            fprintf(stderr, "minibash: syntax error: unclosed quotes\n");
            return -1;
        }

        len += quote_end + 1;
    }

    return len;
}

/*
 * @brief Split the input string into a token list
 * @param str The input string
 * @return A list of tokens, NULL on failure
 **/
t_lst* lexer(char* str) {
    t_lst* token_list = lst_new();

    int prev_type = PIPE;

    while (*str) {
        ssize_t len = token_len(str);
        if (len == -1) {
            lst_del(token_list, free_token);
            return NULL;
        }

        char* token_str = strndup(str, len);
        if (token_str == NULL) {
            lst_del(token_list, free_token);
            perror("minibash: malloc");
            return NULL;
        }

        token_t* token = new_token(token_str);
        if (token == NULL) {
            free(token_str);
            lst_del(token_list, free_token);
            perror("minibash: malloc");
            return NULL;
        }

        if ((prev_type == REDIR && token->type != WORD) ||
            (prev_type == PIPE && token->type == PIPE)) {
            lst_del(token_list, free_token);
            free_token(token);
            return syntax_error(token->str);
        }

        prev_type = token->type;

        if (lst_push_back(token_list, token) != 0) {
            free_token(token);
            lst_del(token_list, free_token);
            perror("minibash: malloc");
            return NULL;
        }

        str += len;
        str = skip_whitespaces(str);
    }

    if (prev_type != WORD) {
        lst_del(token_list, free_token);
        return syntax_error("newline");
    }

    return token_list;
}
