#include "minibash.h"

static void free_cmd(void* content)
{
	cmd_t* cmd = content;
	for (size_t i = 0; cmd->args[i]; i++)
		free(cmd->args[i]);
	free(cmd->args);
	free(cmd);
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Usage: ./minibash \"<commands>\"\n");
		return 1;
	}
	list_t* cmds_lst = parser(argv[1]);
	lst_clear(cmds_lst, free_cmd);
	return 0;
}
