#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    int fd;
    char buffer[1024];
    ssize_t bytesRead;

    // Open the file in read-only mode
    fd = open("../../example.txt", O_RDONLY);

    if(fd == -1)
    {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }

    // Set the file descriptor to non-blocking mode
    int flags = fcntl(fd, F_GETFL);
    if(flags == -1)
    {
        perror("Error getting file descriptor flags");
        close(fd);
        return EXIT_FAILURE;
    }

    if(fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("Error setting file descriptor to non-blocking mode");
        close(fd);
        return EXIT_FAILURE;
    }

    // Read from the file (will not block due to O_NONBLOCK)
    bytesRead = read(fd, buffer, sizeof(buffer));

    if(bytesRead == -1)
    {
        if(errno == EAGAIN)
        {
            printf("No data available (EAGAIN)\n");
        }
        else
        {
            perror("Error reading from the file");
            close(fd);
            return EXIT_FAILURE;
        }
    }
    else
    {
        buffer[bytesRead] = '\0';
        printf("Read %zd bytes:\n%s\n", bytesRead, buffer);
    }

    if(close(fd) == -1)
    {
        perror("Error closing the file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
