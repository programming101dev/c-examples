#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    gid_t egid;

    egid = getegid();
    printf("Effective GID: %d\n", egid);

    return EXIT_SUCCESS;
}
