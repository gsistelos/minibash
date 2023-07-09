#ifndef MINIBASH_H
# define MINIBASH_H

# define STR 1
# define REDIR 2
# define PIPE 3

# include <stdlib.h>
# include <stdio.h>

typedef struct list
{
	void*			content;
	struct list*	next;
}	list_t;

typedef struct s_token
{
	char*	str;
	int		type;
}	t_token;

typedef struct s_cmd
{
	char**	args;
	int		input;
	int		output;
}	t_cmd;

/* utils */
void	lst_clear(list_t* lst, void (*f)(void*));
list_t* lst_last(list_t* lst);
list_t* lst_new(void* content);
int 	lst_push(list_t** lst, list_t* new);

#endif /* MINIBASH_H */
