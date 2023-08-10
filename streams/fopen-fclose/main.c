#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
    const char *filename;
    FILE *file;

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    filename = argv[1];
    file = fopen(filename, "rb");

    if(file == NULL)
    {
        fprintf(stderr, "Error opening the file '%s': %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }

    printf("Successfully opened file: %s\n", filename);

    if(fclose(file) != 0)
    {
        fprintf(stderr, "Error closing the file '%s': %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
