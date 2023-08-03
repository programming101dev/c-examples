#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    const size_t BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];

    fputs("Please input some lines, end with Ctrl-D:\n", stdout);

    while(fgets(buffer, BUFFER_SIZE, stdin) != NULL)
    {
        fputs("You typed: ", stdout);
        fputs(buffer, stdout);
    }

    return EXIT_SUCCESS;
}
