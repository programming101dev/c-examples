#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    pid_t pid = getpid(); // Get the PID of the current process

    pid_t pgid = getpgid(pid);
    if(pgid == -1) {
        perror("Error getting PGID");
        return 1;
    }

    printf("Process ID (PID): %d\n", pid);
    printf("Process Group ID (PGID) of the process with PID %d: %d\n", pid, pgid);

    return EXIT_SUCCESS;
}
