#ifndef MINIBASH_H
# define MINIBASH_H

# define STR 1
# define REDIR 2
# define PIPE 3

# include <stdio.h>

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

#endif /* MINIBASH_H */
