#include "minibash.h"

static size_t args_len(list_t* tokens_lst) {
    size_t len = 0;
    while (tokens_lst) {
        token_t* token = tokens_lst->content;
        if (token->type == PIPE)
            break;

        if (token->type == REDIR)
            tokens_lst = tokens_lst->next;
        else
            len++;

        tokens_lst = tokens_lst->next;
    }
    return len;
}

static int heredoc(char* end) {
    int fd[2];
    char buffer[4097];

    pipe(fd);
    size_t line = 0;
    while (1) {
        write(STDOUT_FILENO, ">", 1);
        int bytes_read = read(STDIN_FILENO, buffer, 4096);
        if (bytes_read < 1) {
            fprintf(stderr, "\nminibash: warning: here-document at line %li delimited by end-of-file (wanted '%s')\n", line, end);
            break;
        }
        buffer[bytes_read] = '\0';

        if (buffer[bytes_read - 1] == '\n' && !strncmp(end, buffer, bytes_read - 1))
            break;
        write(fd[1], buffer, bytes_read);
        line++;
    }
    close(fd[1]);
    return fd[0];
}

static int redirect(list_t* tokens_lst, int* input, int* output) {
    char* redir = ((token_t*)tokens_lst->content)->str;
    char* file = ((token_t*)tokens_lst->next->content)->str;

    if (!strcmp(redir, ">>"))
        *output = open(file, O_WRONLY | O_APPEND | O_CREAT, 0644);
    else if (!strcmp(redir, "<<"))
        *input = heredoc(file);
    else if (redir[0] == '>')
        *output = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    else
        *input = open(file, O_RDONLY);

    if (*input == -1 || *output == -1) {
        fprintf(stderr, "minibash: %s: %s\n", file, strerror(errno));
        return 1;
    }
    return 0;
}

static list_t* cmd_info(list_t* tokens_lst, char*** args, int* input, int* output) {
    *input = 0;
    *output = 1;

    size_t i = args_len(tokens_lst);

    args[0] = malloc(sizeof(char*) * (i + 1));
    i = 0;
    while (tokens_lst) {
        token_t* token = tokens_lst->content;
        if (token->type == PIPE)
            break;

        if (token->type == REDIR) {
            redirect(tokens_lst, input, output);
            tokens_lst = tokens_lst->next;
        } else
            args[0][i++] = strdup(token->str);

        tokens_lst = tokens_lst->next;
    }
    args[0][i] = NULL;
    return tokens_lst;
}

list_t* interpreter(list_t* tokens_lst) {
    // TOOD: refactor interpreter

    list_t* cmds_lst = NULL;
    char** args = NULL;
    int input, output;

    tokens_lst = cmd_info(tokens_lst, &args, &input, &output);
    lst_push(&cmds_lst, lst_new(new_cmd(args, input, output)));
    while (tokens_lst) {
        tokens_lst = tokens_lst->next;
        tokens_lst = cmd_info(tokens_lst, &args, &input, &output);
        lst_push(&cmds_lst, lst_new(new_cmd(args, input, output)));
    }
    return cmds_lst;
}
