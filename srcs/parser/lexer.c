#include "minibash.h"

static token_t* new_token(char* str)
{
	token_t* token = malloc(sizeof(token_t));
	token->str = str;
	if (str[0] == '|')
		token->type = PIPE;
	else if (strchr("><", str[0]))
		token->type = REDIR;
	else
		token->type = WORD;
	return token;
}

list_t* lexer(char* str)
{
	list_t* tokens = NULL;

	while (isspace(*str))
		str++;
	while (*str) {
		size_t len = 0;
		if (!strncmp(str, ">>", 2) || !strncmp(str, "<<", 2))
			len = 2;
		else if (strchr("|><", *str))
			len = 1;
		else {
			while (!isspace(str[len]) && !strchr("|><", str[len])) {
				len += quotes_len(str + len);
				if (str[len] != '\0')
					len++;
			}
		}

		lst_push(&tokens, lst_new(new_token(strndup(str, len))));
		str += len;
		while (isspace(*str))
			str++;
	}
	return tokens;
}
