#include <stdio.h>
#include <stdlib.h>


#define BUFFER_SIZE 256


int main(void)
{
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    buffer[0] = '\0';

    // Read up to BUFFER_SIZE - 1 bytes from stdin, leaving room for a null terminator
    // NOTE: it will not return when \n is presed, you have to reach the size or end of file
    //       before fread returns
    while((bytesRead = fread(buffer, 1, BUFFER_SIZE - 1, stdin)) > 0)
    {
        // Add a null terminator to the end of the buffer
        buffer[bytesRead] = '\0';

        // Print the buffer
        fputs("You typed: ", stdout);
        fwrite(buffer, 1, bytesRead, stdout);

        // Clear the buffer
        buffer[0] = '\0';
    }

    return EXIT_SUCCESS;
}
