#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    errno = 0;
    fgetc(stdout);

    if(errno != 0)
    {
        clearerr(stderr);
        perror("error with fgetc");

        if(ferror(stderr) != 0)
        {
            perror("error with perror");
        }
    }

    return EXIT_SUCCESS;
}
