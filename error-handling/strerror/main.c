#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    errno = 0;
    fgetc(stdout);

    if(errno != 0)
    {
        const char *msg;
        int current_erro_no;

        current_erro_no = errno;
        errno = 0;
        msg = strerror(current_erro_no);

        if(errno != 0)
        {
            perror("strerror");
            return EXIT_FAILURE;
        }

        errno = 0;

        if(fprintf(stderr, "There was an error with fgetc: %s\n", msg) < 0)
        {
            perror("fprintf");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
