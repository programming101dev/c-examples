#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    errno_t *data;

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