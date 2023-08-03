#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    const char buffer[] = "This is an attempt to write.";
    FILE *file;

    file = fopen("../../example.txt", "r");

    if(file == NULL)
    {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }

    // Try to write to the file (this will fail in read mode)
    if(fputs(buffer, file) == EOF)
    {
        if(ferror(file))
        {
            perror("Error writing to the file");
        }
        else
        {
            printf("End of file or other write error\n");
        }
    }

    // Close the file
    fclose(file);

    return EXIT_SUCCESS;
}