#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    const char *filename = "example.txt";
    FILE *file;

    file = fopen(filename, "w");

    if(file == NULL)
    {
        perror("Error creating file");
        return EXIT_FAILURE;
    }

    fprintf(file, "This is a sample file.\n");
    fclose(file);

    // Check if the file exists before deletion
    if(access(filename, F_OK) == -1)
    {
        printf("File '%s' does not exist.\n", filename);
        return EXIT_FAILURE;
    }

    // Delete the file using unlink
    if(unlink(filename) == -1)
    {
        perror("Error deleting file");
        return EXIT_FAILURE;
    }

    // Check if the file still exists after deletion
    if(access(filename, F_OK) == -1)
    {
        printf("File '%s' successfully deleted.\n", filename);
    }
    else
    {
        printf("File '%s' was not deleted.\n", filename);
    }

    return EXIT_SUCCESS;
}
