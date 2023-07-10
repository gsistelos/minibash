#include "minibash.h"

size_t quotes_len(char* str)
{
	size_t len = 0;
	if (str[len] == '\'') {
		len++;
		while (str[len] && str[len] != '\'')
			len++;
	} else if (str[len] == '\"') {
		len++;
		while (str[len] && str[len] != '\"')
			len++;
	}
	return len;
}
