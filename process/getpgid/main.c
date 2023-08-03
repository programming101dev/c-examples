#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(void)
{
    pid_t pgid;

    pgid = getpgid(0);

    if (pgid == -1)
    {
        perror("Error getting Process Group ID");
        return EXIT_FAILURE;
    }

    printf("Process Group ID (PGID): %d\n", pgid);

    return EXIT_SUCCESS;
}
