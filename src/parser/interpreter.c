#include "minibash.h"

#define BUFFER_SIZE 4096

/*
 * @brief Reads from stdin until the end of the heredoc is reached
 * @param end The end of the heredoc
 * @return The file descriptor of the pipe, or -1 on error
 */
static int heredoc(char* end) {
    int fd[2];
    char buffer[BUFFER_SIZE + 1];

    if (pipe(fd) == -1)
        return -1;

    size_t line = 0;
    while (1) {
        if (write(STDOUT_FILENO, ">", 1) == -1)
            return -1;

        size_t bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (bytes_read == (size_t)-1)
            return -1;
        else if (bytes_read == 0) {
            fprintf(stderr, "\nminibash: warning: here-document at line %li delimited by end-of-file (wanted '%s')\n", line, end);
            break;
        }

        buffer[bytes_read] = '\0';

        if (buffer[bytes_read - 1] == '\n' && !strncmp(end, buffer, bytes_read - 1))
            break;

        if (write(fd[1], buffer, bytes_read) == -1)
            return -1;

        line++;
    }

    close(fd[1]);
    return fd[0];
}

static int redirect(list_t* tokens_lst, int* input, int* output) {
    token_t* redir = tokens_lst->data;
    token_t* file = tokens_lst->next->data;

    if (!strcmp(redir->str, ">>"))
        *output = open(file->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
    else if (!strcmp(redir->str, "<<")) {
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
 * @param tokens_lst The token list
 * @param input A pointer to the input fd
 * @param output A pointer to the output fd
 * @return Quantity of non redirect tokens until the pipe or -1 if an error occured
 **/
static size_t redirect_to_pipe(list_t* tokens_lst, int* input, int* output) {
    size_t len = 0;
    while (tokens_lst) {
        token_t* token = tokens_lst->data;
        if (token->type == PIPE)
            break;

        if (token->type == REDIR) {
            if (redirect(tokens_lst, input, output))
                return -1;
            tokens_lst = tokens_lst->next;
        } else
            len++;

        tokens_lst = tokens_lst->next;
    }

    return len;
}

/*
 * @brief Copy the non redirect tokens from the token list to an array and walk the token list head to the next pipe or NULL
 * @param tokens_lst The token list
 * @param len The quantity of non redirect arguments to copy
 * @return The arguments array or NULL if an error occured
 **/
char** copy_args(list_t** tokens_lst, size_t len) {
    char** args = malloc(sizeof(char*) * (len + 1));
    if (args == NULL)
        return NULL;

    size_t i = 0;
    while (*tokens_lst) {
        token_t* token = (*tokens_lst)->data;
        if (token->type == PIPE) {
            *tokens_lst = (*tokens_lst)->next;
            break;
        }

        if (token->type == REDIR)
            *tokens_lst = (*tokens_lst)->next;
        else if (token->str != NULL) {
            args[i] = strdup(token->str);
            if (args[i] == NULL) {
                matrix_free((void**)args);
                return NULL;
            }
            i++;
        }

        *tokens_lst = (*tokens_lst)->next;
    }

    args[len] = NULL;

    return args;
}

/*
 * @brief Create a cmd list from a token list
 * @param tokens_list The token list
 * @return The cmd list or NULL if an error occured
 **/
list_t* interpreter(list_t* tokens_lst) {
    list_t* cmds_lst = NULL;

    while (tokens_lst) {
        int input = STDIN_FILENO;
        int output = STDOUT_FILENO;

        size_t args_len = redirect_to_pipe(tokens_lst, &input, &output);
        if (args_len == (size_t)-1) {
            list_clear(cmds_lst, free_cmd);
            return NULL;
        }

        char** args = copy_args(&tokens_lst, args_len);
        if (args == NULL) {
            list_clear(cmds_lst, free_cmd);
            perror("minibash: malloc");
            return NULL;
        }

        cmd_t* cmd = new_cmd(args, input, output);
        if (cmd == NULL) {
            matrix_free((void**)args);
            list_clear(cmds_lst, free_cmd);
            perror("minibash: malloc");
            return NULL;
        }

        if (list_push_back(&cmds_lst, list_new(cmd))) {
            free_cmd(cmd);
            list_clear(cmds_lst, free_cmd);
            perror("minibash: malloc");
            return NULL;
        }
    }

    return cmds_lst;
}
