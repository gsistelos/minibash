#ifndef MINIBASH_H
#define MINIBASH_H

#define WORD 1
#define REDIR 2
#define PIPE 3

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct list {
    void* content;
    struct list* next;
} list_t;

typedef struct token {
    char* str;
    int type;
} token_t;

typedef struct cmd {
    char** args;
    int input;
    int output;
} cmd_t;

// parser

int expansor(list_t* tokens_lst);
list_t* interpreter(list_t* tokens_lst);
list_t* lexer(char* str);
list_t* parser(char* str);

// test

void print_cmds(list_t* cmds_lst);
void print_fd(int fd);
void print_tokens(list_t* tokens_lst);

// utils

void free_cmd(void* cmd);
void free_token(void* token);
void lst_clear(list_t* lst, void (*f)(void*));
list_t* lst_last(list_t* lst);
list_t* lst_new(void* content);
int lst_push(list_t** lst, list_t* new);
cmd_t* new_cmd(char** args, int input, int output);
token_t* new_token(char* str);
size_t quotes_len(char* str);

#endif /* MINIBASH_H */
