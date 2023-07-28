#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    const char *filename;
    FILE *file;

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    filename = argv[1];

    file = fopen(filename, "rb");

    if(file == NULL)
    {
        fprintf(stderr, "Error opening the file: %s\n", filename);
        return EXIT_FAILURE;
    }

    printf("Successfully opened file: %s\n", filename);

    fclose(file);

    return EXIT_SUCCESS;
}
