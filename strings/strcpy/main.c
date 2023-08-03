#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    const char *source = "Hello, World!";
    size_t destination_size;
    char *destination;

    destination_size = strlen(source) + 1;
    destination = malloc(destination_size);

    if(destination == NULL)
    {
        perror("Memory allocation failed\n");

        return EXIT_FAILURE;
    }

    strcpy(destination, source);
    printf("Copied string: %s\n", destination);
    free(destination);

    return EXIT_SUCCESS;
}
