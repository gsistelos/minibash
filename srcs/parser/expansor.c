#include "minibash.h"

static char* expand(char* str)
{
	if (!strcmp(str, "AMB"))
		return NULL;
	return strdup(str);
}

int expansor(list_t* tokens)
{
	token_t* token = tokens->content;
	char* new_str = expand(token->str);
	free(token->str);
	token->str = new_str;
	int prev_type = token->type;
	tokens = tokens->next;

	while (tokens) {
		token = tokens->content;

		if ((prev_type == REDIR && token->type != WORD)
			|| (prev_type == PIPE && token->type == PIPE)) {
			fprintf(stderr, "minibash: syntax error near unexpected token '%s'\n", token->str);
			return 1;
		}
		new_str = expand(token->str);
		if (new_str == NULL && prev_type == REDIR) {
			fprintf(stderr, "minibash: %s: ambigous redirect\n", token->str);
			return 1;
		}
		free(token->str);
		token->str = new_str;
		prev_type = token->type;
		tokens = tokens->next;
	}
	if (prev_type != WORD) {
		fprintf(stderr, "minibash: syntax error near unexpected token '%s'\n", token->str);
		return 1;
	}
	return 0;
}
