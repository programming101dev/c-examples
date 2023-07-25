#include <errno.h>
#include <stdint.h>
#include <stdio.h>
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
        perror("error with malloc");
    }

    return EXIT_SUCCESS;
}