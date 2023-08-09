#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


int main(void)
{
    void *data;

    errno = 0;
    data = malloc(SIZE_MAX);

    if(data)
    {
        free(data);
    }
    else
    {
        fprintf(stderr, "errno: %d\n", errno);
    }

    return EXIT_SUCCESS;
}
