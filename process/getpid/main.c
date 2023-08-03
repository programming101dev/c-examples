#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    pid_t pid;

    pid = getpid();
    printf("Process ID (PID): %d\n", pid);

    return EXIT_SUCCESS;
}
