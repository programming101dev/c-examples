#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    printf("STDIN_FILENO: %d\n", STDIN_FILENO);
    printf("STDOUT_FILENO: %d\n", STDOUT_FILENO);
    printf("STDERR_FILENO: %d\n", STDERR_FILENO);

    return EXIT_SUCCESS;
}
