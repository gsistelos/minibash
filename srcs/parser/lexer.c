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

/*
 * @brief Split the input string into a token list, checks for syntax errors
 * @param str The input string
 * @return A list of tokens, or NULL if an error occurred
 */
list_t* lexer(char* str) {
    list_t* tokens_lst = NULL;
    int prev_type = PIPE;

    while (*str) {
        size_t len = token_len(str);
        if (len == (size_t)-1) {
            list_clear(tokens_lst, free_token);
            fprintf(stderr, "minibash: syntax error: unclosed quotes\n");
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
            list_clear(tokens_lst, free_token);
            fprintf(stderr, "minibash: syntax error near unexpected token '%s'\n", token->str);
            return NULL;
        }

        prev_type = token->type;

        if (list_push_back(&tokens_lst, list_new(token))) {
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
