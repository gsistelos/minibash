#include "minibash.h"

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Usage: ./minibash \"<commands>\"\n");
		return 1;
	}
	list_t* cmds = parser(argv[1]);
	(void)cmds;
	return 0;
}
