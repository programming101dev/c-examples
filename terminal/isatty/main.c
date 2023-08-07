#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int fd = fileno(stdin); // Get the file descriptor for stdin

    if(isatty(fd)) {
        printf("stdin is a terminal device.\n");
    } else {
        printf("stdin is not a terminal device.\n");
    }

    return EXIT_SUCCESS;
}
