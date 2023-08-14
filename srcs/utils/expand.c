#include "minibash.h"

char* expand(char* str) {
    // TODO: expand $VARIABLES and remove quotes

    if (!strcmp(str, "AMB"))
        return NULL;

    return strdup(str);
}
