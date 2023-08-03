#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    const char *source = "Hello, World!";
    size_t to_copy;
    size_t destination_size;
    char *destination;

    to_copy = 8;
    destination_size = to_copy + 1;
    destination = malloc(destination_size);

    if(destination == NULL)
    {
        perror("Memory allocation failed\n");

        return EXIT_FAILURE;
    }

    strncpy(destination, source, to_copy);
    destination[to_copy] = '\0';
    printf("Copied string: \"%s\"\n", destination);
    free(destination);

    return EXIT_SUCCESS;
}
