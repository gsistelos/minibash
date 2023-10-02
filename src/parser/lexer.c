#include "minibash.h"

/* @brief Get the length of the next token in a string
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
list_t* lexer(char* str) {
    list_t* tokens_lst = NULL;
    int prev_type = PIPE;

    while (*str) {
        ssize_t len = token_len(str);
        if (len == -1) {
            list_clear(tokens_lst, free_token);
            return NULL;
        }

        char* token_str = strndup(str, len);
        if (token_str == NULL) {
            list_clear(tokens_lst, free_token);
            perror("minibash: malloc");
            return NULL;
        }

        token_t* token = new_token(token_str);
        if (token == NULL) {
            free(token_str);
            list_clear(tokens_lst, free_token);
            perror("minibash: malloc");
            return NULL;
        }

        if ((prev_type == REDIR && token->type != WORD) || (prev_type == PIPE && token->type == PIPE)) {
            free_token(token);
            list_clear(tokens_lst, free_token);
            fprintf(stderr, "minibash: syntax error near unexpected token '%s'\n", token->str);
            return NULL;
        }

        prev_type = token->type;

        if (list_push_back(&tokens_lst, list_new(token)) != 0) {
            free_token(token);
            list_clear(tokens_lst, free_token);
            perror("minibash: malloc");
            return NULL;
        }

        str += len;
        str = skip_whitespaces(str);
    }

    if (prev_type != WORD) {
        list_clear(tokens_lst, free_token);
        fprintf(stderr, "minibash: syntax error near unexpected token 'newline'\n");
        return NULL;
    }

    return tokens_lst;
}
