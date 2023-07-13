#ifndef MINIBASH_H
# define MINIBASH_H

# define WORD 1
# define REDIR 2
# define PIPE 3

# include <stdlib.h>
# include <stdio.h>
# include <ctype.h>
# include <string.h>

typedef struct list
{
	void*			content;
	struct list*	next;
}	list_t;

typedef struct token
{
	char*	str;
	int		type;
}	token_t;

typedef struct cmd
{
	char**	args;
	int		input;
	int		output;
}	cmd_t;

/* parser */
int		expansor(list_t* tokens_lst);
list_t*	interpreter(list_t* tokens_lst);
list_t*	lexer(char* str);
list_t*	parser(char* str);

/* test */
void	print_cmds(list_t* cmds_lst);
void	print_tokens(list_t* tokens_lst);


/* utils */
void	lst_clear(list_t* lst, void (*f)(void*));
list_t*	lst_last(list_t* lst);
list_t*	lst_new(void* content);
int		lst_push(list_t** lst, list_t* new);
size_t	quotes_len(char* str);

#endif /* MINIBASH_H */
