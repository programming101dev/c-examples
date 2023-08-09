#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    char str[] = "This,is,a,test,string";
    const char *delimiter = ",";
    char *token;
    char *saveptr; // Used to save the context between calls to strtok_r

    // First call to strtok_r with the input string and delimiter
    token = strtok_r(str, delimiter, &saveptr);

    while(token != NULL)
    {
        printf("Token: %s\n", token);

        // Subsequent calls to strtok_r with NULL as the first argument
        // to continue tokenizing the same string
        token = strtok_r(NULL, delimiter, &saveptr);
    }

    // NOTE that strtok_r is destructive to the original string
    printf("\nOriginal string \"%s\"\n", str);

    return EXIT_SUCCESS;
}
