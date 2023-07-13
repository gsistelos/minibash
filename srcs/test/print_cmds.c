#include "minibash.h"

void print_cmds(list_t* cmds_lst)
{
	while (cmds_lst) {
		cmd_t* cmd = cmds_lst->content;

		for (size_t i = 0; cmd->args[i]; i++) {
			printf("%s | ", cmd->args[i]);
		}

		printf("%i | %i\n", cmd->input, cmd->output);

		cmds_lst = cmds_lst->next;
	}
}
