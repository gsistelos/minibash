#include "minibash.h"

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Usage: ./minibash \"<commands>\"\n");
		return 1;
	}
	(void)argv;
	return 0;
}
