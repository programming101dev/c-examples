#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(void)
{
    char template[] = "/tmp/mytempfileXXXXXX";
    const char *data = "This is some data that needs to be written to the file.\n";
    int fd;

    printf("Creating a temporary file...\n");

    fd = mkstemp(template);

    if(fd == -1)
    {
        perror("Error creating temporary file");
        return EXIT_FAILURE;
    }

    if(dprintf(fd, "%s", data) < 0)
    {
        perror("Error writing to temporary file");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Flushing data to disk...\n");
    sync();
    printf("Data successfully flushed to disk.\n");

    if(close(fd) == -1)
    {
        perror("Error closing file descriptor");
        return EXIT_FAILURE;
    }

    printf("Deleting the temporary file...\n");

    if(unlink(template) == -1)
    {
        perror("Error deleting temporary file");
        return EXIT_FAILURE;
    }

    printf("Temporary file deleted successfully.\n");

    return EXIT_SUCCESS;
}
