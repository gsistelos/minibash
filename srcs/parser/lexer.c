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

static size_t token_len(char* str)
{
	if (!strncmp(str, ">>", 2) || !strncmp(str, "<<", 2))
		return 2;
	else if (strchr("|><", *str))
		return 1;
	size_t len = 0;
	while (!isspace(str[len]) && !strchr("|><", str[len])) {
		len += quotes_len(str + len);
		if (str[len] != '\0')
			len++;
	}
	return len;
}

list_t* lexer(char* str)
{
	list_t* tokens_lst = NULL;

	while (isspace(*str))
		str++;
	while (*str) {
		size_t len = token_len(str);
		lst_push(&tokens_lst, lst_new(new_token(strndup(str, len))));
		str += len;
		while (isspace(*str))
			str++;
	}
	return tokens_lst;
}
