#include "minibash.h"

void safe_close(int in, int out) {
    if (in != STDIN_FILENO)
        close(in);
    if (out != STDOUT_FILENO)
        close(out);
}
