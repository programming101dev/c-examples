#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    const char *source = "Hello, World!";
    char *duplicate;

    duplicate = strdup(source);

    if(duplicate == NULL)
    {
        perror("Memory allocation failed for duplicate\n");

        return EXIT_FAILURE;
    }

    printf("Duplicated string: \"%s\"\n", duplicate);
    free(duplicate);

    return EXIT_SUCCESS;
}
