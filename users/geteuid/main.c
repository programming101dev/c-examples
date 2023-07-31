#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    uid_t euid;

    euid = geteuid();
    printf("Effective UID: %d\n", euid);

    return EXIT_SUCCESS;
}
