#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    int fd;
    FILE* file;
    char buffer[256];

    fd = open("../../example.txt", O_RDONLY);

    if(fd == -1)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    file = fdopen(fd, "r");

    if(file == NULL)
    {
        perror("Error creating FILE* stream");
        close(fd);
        return EXIT_FAILURE;
    }

    while(fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer); // Print the content to stdout
    }

    if(fclose(file) == EOF)
    {
        perror("Error closing FILE* stream");
        return EXIT_FAILURE;
    }

    // close was called by fclose

    return EXIT_SUCCESS;
}
