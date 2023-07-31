#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    char str1[50] = "Hello, ";
    const char *str2 = "World!";

    strncat(str1, str2, 3);
    printf("Concatenated string: \"%s\"\n", str1);

    return EXIT_SUCCESS;
}
