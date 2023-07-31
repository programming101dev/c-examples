#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    const char str[] = "Hello, World!";
    size_t length;

    length = strlen(str);
    printf("Length of \"%s\" is %zu\n", str, length);

    return EXIT_SUCCESS;
}
