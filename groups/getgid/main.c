#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    gid_t gid;

    gid = getgid();
    printf("Real GID: %d\n", gid);

    return EXIT_SUCCESS;
}
