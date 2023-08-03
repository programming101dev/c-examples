#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    char template[] = "/tmp/temp_file_XXXXXX";
    int fd;

    // Create a temporary file using mkstemp
    fd = mkstemp(template);

    if(fd == -1)
    {
        perror("Error creating temporary file");
        return EXIT_FAILURE;
    }

    printf("Created temporary file %s\n", template);

    // Delete the temporary file using unlink
    if(unlink(template) == -1)
    {
        perror("Error deleting temporary file");
        return EXIT_FAILURE;
    }

    printf("Temporary file deleted successfully.\n");

    return EXIT_SUCCESS;
}
