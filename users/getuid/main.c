#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    uid_t uid;

    uid = getuid();
    printf("Real UID: %d\n", uid);

    return EXIT_SUCCESS;
}
