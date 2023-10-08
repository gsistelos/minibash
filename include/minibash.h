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
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libgs.h"

enum token_type_e {
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

// builtin

int builtin_cd(cmd_t* cmd);
int builtin_env(cmd_t* cmd);
int builtin_exit(cmd_t* cmd);
int builtin_export(cmd_t* cmd);
int builtin_null(cmd_t* cmd);
int builtin_unset(cmd_t* cmd);
void* get_builtin(char* cmd);

// executor

pid_t executor(list_t* cmd_list);
int execve_cmd(cmd_t* cmd);
pid_t fork_exec(int (*exec_func)(cmd_t* cmd), cmd_t* cmd, int bridge_pipe);
pid_t run_cmd(cmd_t* cmd);
void wait_pids(pid_t* pid, size_t size);

// parser

char* expand(char* str);
int expansor(list_t* token_list);
list_t* interpreter(list_t* token_list);
list_t* lexer(char* str);
list_t* parser(char* str);

// signal

int setup_signals(void);

// utils

void free_cmd(void* ptr);
void free_token(void* ptr);
char* get_path(char* cmd);
cmd_t* new_cmd(char** args, int input, int output);
token_t* new_token(char* str);
ssize_t quotes_len(char* str);
char* skip_whitespaces(char* str);

extern int g_status_code;

extern char** environ;

#endif  // MINIBASH_H
