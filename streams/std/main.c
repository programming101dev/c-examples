#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    int stdin_fd;
    int stdout_fd;
    int stderr_fd;

    stdin_fd = fileno(stdin);
    stdout_fd = fileno(stdout);
    stderr_fd = fileno(stderr);
    printf("File descriptor for stdin: %d\n", stdin_fd);
    printf("File descriptor for stdout: %d\n", stdout_fd);
    printf("File descriptor for stderr: %d\n", stderr_fd);

    return EXIT_SUCCESS;
}
