#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
        const char *msg;

        msg = strerror(errno);
        fprintf(stderr, "error with malloc: %s\n", msg);
    }

    return EXIT_SUCCESS;
}