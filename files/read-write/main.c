#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    const unsigned int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while((bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
    {
        ssize_t bytesWritten;

        bytesWritten = write(STDOUT_FILENO, buffer, bytesRead);

        if(bytesWritten != bytesRead)
        {
            perror("Error writing to stdout");
            return EXIT_FAILURE;
        }
    }

    if(bytesRead == -1)
    {
        perror("Error reading from stdin");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
