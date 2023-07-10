#include "minibash.h"

static void free_token(void* content)
{
	token_t* token = content;
	free(token->str);
	free(token);
}

list_t* parser(char* str)
{
	list_t* tokens_lst = lexer(str);
	if (expansor(tokens_lst)) {
		lst_clear(tokens_lst, free_token);
		return NULL;
	}
	list_t* cmds = NULL;
	lst_clear(tokens_lst, free_token);
	return cmds;
}
