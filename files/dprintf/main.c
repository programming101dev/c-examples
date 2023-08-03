#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    dprintf(STDOUT_FILENO, "Hello, %s! This is a dprintf example for stdout.\n", "World");

    return EXIT_SUCCESS;
}
