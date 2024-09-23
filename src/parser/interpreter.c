#include "liblst.h"
#include "minibash.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <readline/readline.h>

static void eof_warning(size_t line, char* wanted) {
    fprintf(stderr,
            "minibash: warning: here-document at line %li delimited by "
            "end-of-file (wanted '%s')\n",
            line, wanted);
}

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
            eof_warning(i, end);
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

static int redirect(t_node* token_node, int* input, int* output) {
    token_t* redir = token_node->data;
    token_t* file = token_node->next->data;

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
 * @brief Iterate over the token list until a pipe
 *        is found, setting the input and output
 * @param token_node Head of the token list
 * @param input A pointer to the input fd
 * @param output A pointer to the output fd
 * @return Quantity of non redirect tokens until
 *         the pipe or -1 if an error occured
 **/
static ssize_t redirect_to_pipe(t_node* token_node, int* input, int* output) {
    ssize_t len = 0;
    while (token_node) {
        token_t* token = token_node->data;
        if (token->type == PIPE)
            break;

        if (token->type == REDIR) {
            if (redirect(token_node, input, output) != 0)
                return -1;
            token_node = token_node->next;
        } else if (token->str != NULL)
            len++;

        token_node = token_node->next;
    }

    return len;
}

/*
 * @brief Copy the non redirect tokens from the token list to
 *        an array and walk the token list head to the next pipe or NULL
 * @param token_node Head of the token list
 * @param len The quantity of non redirect arguments to copy
 * @return The arguments array or NULL if an error occured
 **/
static char** copy_args(t_node** token_node, ssize_t len) {
    char** args = malloc(sizeof(char*) * (len + 1));
    if (args == NULL) {
        perror("minibash: malloc");
        return NULL;
    }

    size_t i = 0;
    while (*token_node) {
        token_t* token = (*token_node)->data;
        if (token->type == PIPE) {
            *token_node = (*token_node)->next;
            break;
        }

        if (token->type == REDIR)
            *token_node = (*token_node)->next;
        else if (token->str != NULL) {
            args[i] = strdup(token->str);
            if (args[i] == NULL) {
                gs_matrix_free((void**)args);
                perror("minibash: malloc");
                return NULL;
            }
            i++;
        }

        *token_node = (*token_node)->next;
    }

    args[len] = NULL;

    return args;
}

/*
 * @brief Create a cmd list from a token list
 * @param tokens_node Head of the token list
 * @return The cmd list or NULL if an error occured
 **/
t_lst* interpreter(t_node* token_node) {
    t_lst* cmd_list = lst_new();

    while (token_node) {
        int input = STDIN_FILENO;
        int output = STDOUT_FILENO;

        ssize_t args_len = redirect_to_pipe(token_node, &input, &output);
        if (args_len == -1) {
            lst_del(cmd_list, free_cmd);
            return NULL;
        }

        char** args = copy_args(&token_node, args_len);
        if (args == NULL) {
            lst_del(cmd_list, free_cmd);
            return NULL;
        }

        cmd_t* cmd = new_cmd(args, input, output);
        if (cmd == NULL) {
            gs_matrix_free((void**)args);
            lst_del(cmd_list, free_cmd);
            perror("minibash: malloc");
            return NULL;
        }

        if (lst_push_back(cmd_list, cmd) != 0) {
            free_cmd(cmd);
            lst_del(cmd_list, free_cmd);
            perror("minibash: malloc");
            return NULL;
        }
    }

    return cmd_list;
}
