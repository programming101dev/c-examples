#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    errno = 0;
    fgetc(stdout);

    if(errno != 0)
    {
        int current_errno;

        current_errno = errno;
        errno = 0;

        if(fprintf(stderr, "errno: %d\n", current_errno) < 0)
        {
            printf("could not fprintf: %d\n", errno);
        }
    }

    return EXIT_SUCCESS;
}
