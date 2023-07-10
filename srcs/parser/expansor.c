#include "minibash.h"

static char* expand(char* str)
{
	if (!strcmp(str, "AMB"))
		return NULL;
	return strdup(str);
}

static int validate_token(token_t* token, int* prev_type)
{
	if ((*prev_type == REDIR && token->type != WORD)
		|| (*prev_type == PIPE && token->type == PIPE)) {
		fprintf(stderr, "minibash: syntax error near unexpected token '%s'\n", token->str);
		return 1;
	}
	char* new_str = expand(token->str);
	if (new_str == NULL && *prev_type == REDIR) {
		fprintf(stderr, "minibash: %s: ambigous redirect\n", token->str);
		return 1;
	}
	free(token->str);
	token->str = new_str;
	*prev_type = token->type;
	return 0;
}

int expansor(list_t* tokens_lst)
{
	int prev_type = PIPE;

	while (tokens_lst->next) {
		if (validate_token(tokens_lst->content, &prev_type))
			return 1;
		tokens_lst = tokens_lst->next;
	}

	token_t* token = tokens_lst->content;
	if (token->type != WORD) {
		fprintf(stderr, "minibash: syntax error near unexpected token '%s'\n", token->str);
		return 1;
	}
	char* new_str = expand(token->str);
	if (new_str == NULL && prev_type == REDIR) {
		fprintf(stderr, "minibash: %s: ambigous redirect\n", token->str);
		return 1;
	}
	free(token->str);
	token->str = new_str;
	return 0;
}
