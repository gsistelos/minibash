#include "minibash.h"

#define BUFFER_SIZE 4096

/*
 * @brief Reads from stdin until the end of the heredoc is reached
 * @param end The end of the heredoc
 * @return The file descriptor of the pipe, or -1 on error
 */
int heredoc(char* end) {
    int fd[2];
    char buffer[BUFFER_SIZE + 1];

    if (pipe(fd) == -1)
        return -1;

    size_t line = 0;
    while (1) {
        if (write(STDOUT_FILENO, ">", 1) == -1)
            return -1;

        size_t bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (bytes_read == (size_t)-1)
            return -1;
        else if (bytes_read == 0) {
            fprintf(stderr, "\nminibash: warning: here-document at line %li delimited by end-of-file (wanted '%s')\n", line, end);
            break;
        }

        buffer[bytes_read] = '\0';

        if (buffer[bytes_read - 1] == '\n' && !strncmp(end, buffer, bytes_read - 1))
            break;

        if (write(fd[1], buffer, bytes_read) == -1)
            return -1;

        line++;
    }

    close(fd[1]);
    return fd[0];
}
