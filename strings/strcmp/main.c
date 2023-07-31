#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    const char *str1 = "Hello, World!";
    const char *str2 = "Hello, World!";
    const char *str3 = "Hello, World?";
    int result;

    result = strcmp(str1, str2);

    if(result == 0)
    {
        printf("str1 and str2 are identical.\n");
    }
    else
    {
        printf("str1 and str2 are different (%d).\n", result);
    }

    result = strcmp(str1, str3);

    if(result == 0)
    {
        printf("str1 and str3 are identical.\n");
    }
    else
    {
        printf("str1 and str3 are different (%d).\n", result);
    }

    return EXIT_SUCCESS;
}
