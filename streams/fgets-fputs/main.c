#include <stdio.h>
#include <stdlib.h>


#define BUFFER_SIZE 256


int main(void)
{
    char buffer[BUFFER_SIZE];

    fputs("Please input some lines, end with Ctrl-D:\n", stdout);

    while(fgets(buffer, BUFFER_SIZE, stdin) != NULL)
    {
        fputs("You typed: ", stdout);
        fputs(buffer, stdout);
    }

    return EXIT_SUCCESS;
}
