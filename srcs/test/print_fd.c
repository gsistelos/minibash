#include "minibash.h"

void print_fd(int fd) {
    char buffer[4097];

    while (1) {
        int bytes_read = read(fd, buffer, 4096);
        if (bytes_read < 1)
            return;

        buffer[bytes_read] = '\0';

        printf("%s", buffer);
    }

    printf("\n");
}
