#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    int fd;

    printf("Opening the file\n");
    fd = open("../../example.txt", O_RDONLY);

    if(fd == -1)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    printf("Closing the file\n");

    if(close(fd) == -1)
    {
        perror("Error closing file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
