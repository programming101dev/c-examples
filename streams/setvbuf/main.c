#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    // Fully buffered - probably won't print because the buffer won't get full
    setvbuf(stdout, NULL, _IOFBF, 0);

    printf("This is part one... ");
    sleep(2);

    printf("and this is part two.\n");
    sleep(2);

    // Line buffered - probably won't print because the buffer won't get full, until it hits \n
    setvbuf(stdout, NULL, _IOLBF, 0);
    printf("This is part three... ");
    sleep(2);

    printf("and this is part four.\n");
    sleep(2);

    // Not buffered - prints out after each printf
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("This is part five... ");
    sleep(2);

    printf("and this is part six.\n");
    sleep(2);

    return EXIT_SUCCESS;
}
