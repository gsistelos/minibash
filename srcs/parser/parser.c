#include "minibash.h"

static void free_token(void* content)
{
	token_t* token = content;
	free(token->str);
	free(token);
}

list_t* parser(char* str)
{
	list_t* tokens = lexer(str);
	list_t* cmds = NULL;
	lst_clear(tokens, free_token);
	return cmds;
}
