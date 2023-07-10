#include "minibash.h"

static cmd_t* new_cmd(char** args, int input, int output)
{
	cmd_t* cmd = malloc(sizeof(cmd_t));
	cmd->args = args;
	cmd->input = input;
	cmd->output = output;
	return cmd;
}

static size_t args_len(list_t* tokens_lst)
{
	size_t len = 0;
	while (tokens_lst) {
		token_t* token = tokens_lst->content;
		if (token->type == PIPE)
			break ;

		if (token->type == REDIR)
			tokens_lst = tokens_lst->next;
		else
			len++;

		tokens_lst = tokens_lst->next;
	}
	return len;
}

static void redirect(char* str, int* input, int* output)
{
	(void)str;
	*input = 0;
	*output = 1;
}

static list_t* cmd_info(list_t* tokens_lst, char*** args, int* input, int* output)
{
	size_t i = args_len(tokens_lst);

	args[0] = malloc(sizeof(char*) * (i + 1));
	i = 0;
	while (tokens_lst) {
		token_t* token = tokens_lst->content;
		if (token->type == PIPE)
			break ;

		if (token->type == REDIR) {
			tokens_lst = tokens_lst->next;
			redirect(token->str, input, output);
		}
		else
			args[0][i++] = strdup(token->str);

		tokens_lst = tokens_lst->next;
	}
	args[0][i] = NULL;
	return tokens_lst;
}

list_t* interpreter(list_t* tokens_lst)
{
	list_t*	cmds_lst = NULL;
	char**	args = NULL;
	int		input, output;

	while (tokens_lst) {
		tokens_lst = cmd_info(tokens_lst, &args, &input, &output);
		lst_push(&cmds_lst, lst_new(new_cmd(args, input, output)));
		if (tokens_lst)
			tokens_lst = tokens_lst->next;
	}
	return cmds_lst;
}
