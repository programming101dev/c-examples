#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void search_for(char needle, const char *haystack);


int main(void)
{
    const char *str = "Hello, World!";

    search_for('o', str);
    search_for('x', str);

    return EXIT_SUCCESS;
}


static void search_for(char needle, const char *haystack)
{
    char *found_char;

    found_char = strchr(haystack, needle);

    if(found_char != NULL)
    {
        printf("Found character '%c' at position %ld\n", needle, found_char - haystack);
    }
    else
    {
        printf("Character '%c' not found.\n", needle);
    }
}
