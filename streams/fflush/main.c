#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    // Fully buffered - probably won't print because the buffer won't get full
    setvbuf(stdout, NULL, _IOFBF, 0);

    printf("This is part one... ");
    sleep(2);

    // flush the stream to force the output
    fflush(stdout);

    printf("and this is part two.\n");
    sleep(2);

    // no fflush needed - it happens when the program exits.

    return EXIT_SUCCESS;
}
