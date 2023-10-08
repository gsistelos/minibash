#include "minibash.h"

/*
 * @brief Reads from stdin until the end of the heredoc is reached
 * @param end The end of the heredoc
 * @return The file descriptor of the pipe, or -1 on error
 **/
static int heredoc(char* end) {
    int fd[2];

    if (pipe(fd) != 0) {
        perror("minibash: pipe");
        return -1;
    }

    for (size_t i = 0; 1; i++) {
        char* line = readline(">");
        if (errno != 0) {
            perror("minibash: readline");
            return -1;
        }

        if (line == NULL) {
            fprintf(stderr, "\nminibash: warning: here-document at line %li delimited by end-of-file (wanted '%s')\n", i, end);
            break;
        }

        if (strcmp(line, end) == 0) {
            free(line);
            break;
        }

        if (write(fd[1], line, strlen(line)) == -1) {
            free(line);
            perror("minibash: write");
            return -1;
        }

        if (write(fd[1], "\n", 1) == -1) {
            perror("minibash: write");
            return -1;
        }

        free(line);
    }

    close(fd[1]);

    return fd[0];
}

static int redirect(list_t* token_list, int* input, int* output) {
    token_t* redir = token_list->data;
    token_t* file = token_list->next->data;

    if (strcmp(redir->str, ">>") == 0)
        *output = open(file->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
    else if (strcmp(redir->str, "<<") == 0) {
        *input = heredoc(file->str);
        if (*input == -1)
            return -1;
    } else if (redir->str[0] == '>')
        *output = open(file->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    else
        *input = open(file->str, O_RDONLY);

    if (*input == -1 || *output == -1) {
        fprintf(stderr, "minibash: %s: %s\n", file->str, strerror(errno));
        return -1;
    }

    return 0;
}

/*
 * @brief Iterate over the token list until a pipe is found, setting the input and output
 * @param token_list The token list
 * @param input A pointer to the input fd
 * @param output A pointer to the output fd
 * @return Quantity of non redirect tokens until the pipe or -1 if an error occured
 **/
static ssize_t redirect_to_pipe(list_t* token_list, int* input, int* output) {
    ssize_t len = 0;
    while (token_list) {
        token_t* token = token_list->data;
        if (token->type == PIPE)
            break;

        if (token->type == REDIR) {
            if (redirect(token_list, input, output) != 0)
                return -1;
            token_list = token_list->next;
        } else if (token->str != NULL)
            len++;

        token_list = token_list->next;
    }

    return len;
}

/*
 * @brief Copy the non redirect tokens from the token list to an array and walk the token list head to the next pipe or NULL
 * @param token_list The token list
 * @param len The quantity of non redirect arguments to copy
 * @return The arguments array or NULL if an error occured
 **/
char** copy_args(list_t** token_list, ssize_t len) {
    char** args = malloc(sizeof(char*) * (len + 1));
    if (args == NULL) {
        perror("minibash: malloc");
        return NULL;
    }

    size_t i = 0;
    while (*token_list) {
        token_t* token = (*token_list)->data;
        if (token->type == PIPE) {
            *token_list = (*token_list)->next;
            break;
        }

        if (token->type == REDIR)
            *token_list = (*token_list)->next;
        else if (token->str != NULL) {
            args[i] = strdup(token->str);
            if (args[i] == NULL) {
                matrix_free((void**)args);
                perror("minibash: malloc");
                return NULL;
            }
            i++;
        }

        *token_list = (*token_list)->next;
    }

    args[len] = NULL;

    return args;
}

/*
 * @brief Create a cmd list from a token list
 * @param tokens_list The token list
 * @return The cmd list or NULL if an error occured
 **/
list_t* interpreter(list_t* token_list) {
    list_t* cmd_list = NULL;

    while (token_list) {
        int input = STDIN_FILENO;
        int output = STDOUT_FILENO;

        ssize_t args_len = redirect_to_pipe(token_list, &input, &output);
        if (args_len == -1) {
            list_clear(cmd_list, free_cmd);
            return NULL;
        }

        char** args = copy_args(&token_list, args_len);
        if (args == NULL) {
            list_clear(cmd_list, free_cmd);
            return NULL;
        }

        cmd_t* cmd = new_cmd(args, input, output);
        if (cmd == NULL) {
            matrix_free((void**)args);
            list_clear(cmd_list, free_cmd);
            perror("minibash: malloc");
            return NULL;
        }

        if (list_push_back(&cmd_list, list_new(cmd)) != 0) {
            free_cmd(cmd);
            list_clear(cmd_list, free_cmd);
            perror("minibash: malloc");
            return NULL;
        }
    }

    return cmd_list;
}
