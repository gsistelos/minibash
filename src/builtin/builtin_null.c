#include "minibash.h"

int builtin_null(cmd_t* cmd) {
    (void)cmd;
    return 0;
}
