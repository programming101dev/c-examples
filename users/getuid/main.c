#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    uid_t uid = getuid();
    printf("Real User ID: %d\n", uid);

    return EXIT_SUCCESS;
}
