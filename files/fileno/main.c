#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    int fd;

    fd = fileno(stdout);
    dprintf(fd, "Hello, %s! This is a dprintf example for stdout.\n", "World");

    return EXIT_SUCCESS;
}
