#include <stdio.h>
#include <unistd.h>


int main(void)
{
    pid_t pid = getpid(); // Get the PID of the current process

    pid_t pgid = getpgrp();

    printf("Process ID (PID): %d\n", pid);
    printf("Process Group ID (PGID) of the calling process: %d\n", pgid);

    return 0;
}
