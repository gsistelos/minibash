#ifndef MINIBASH_H
#define MINIBASH_H

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libgs.h"

enum token_type {
    WORD,
    REDIR,
    PIPE
};

typedef struct token_s {
    char* str;
    int type;
} token_t;

typedef struct cmd_s {
    char** args;
    int input;
    int output;
} cmd_t;

// executor

pid_t executor(list_t* cmds_lst);

// parser

int expansor(list_t* tokens_lst);
list_t* interpreter(list_t* tokens_lst);
list_t* lexer(char* str);
list_t* parser(char* str);

// signal

int setup_signals(void);

// test

void print_cmds(list_t* cmds_lst);
void print_fd(int fd);
void print_tokens(list_t* tokens_lst);

// utils

char* expand(char* str);
void free_cmd(void* cmd);
void free_token(void* token);
char* get_path(char* cmd);
int heredoc(char* end);
cmd_t* new_cmd(char** args, int input, int output);
token_t* new_token(char* str);
size_t quotes_len(char* str);
void safe_close(int in, int out);
char* skip_whitespaces(char* str);

extern int g_status_code;

#endif  // MINIBASH_H
