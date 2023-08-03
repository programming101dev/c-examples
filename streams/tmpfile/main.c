#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    FILE *tempFile;

    tempFile = tmpfile();

    if(tempFile == NULL)
    {
        perror("Error creating temporary file");
        return EXIT_FAILURE;
    }

    printf("Temp file created\n");

    // The temporary file will be automatically deleted when the program terminates
    // or when you explicitly close the file using fclose:
    fclose(tempFile); // TODO: error handling
    printf("Temp file closed\n");

    return EXIT_SUCCESS;
}
