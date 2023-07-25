#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    gid_t gid = getgid();
    printf("Real Group ID: %d\n", gid);

    return EXIT_SUCCESS;
}
