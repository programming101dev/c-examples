#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void search_for(const char *needle, const char *haystack);


int main(void)
{
    const char *str = "Hello, World!";

    search_for("World!", str);
    search_for("World?", str);

    return EXIT_SUCCESS;
}


static void search_for(const char *needle, const char *haystack)
{
    char *found_substring;

    found_substring = strstr(haystack, needle);

    if(found_substring != NULL)
    {
        printf("Found substring '%s' at position %ld\n", needle, found_substring - haystack);
    }
    else
    {
        printf("Substring '%s' not found.\n", needle);
    }
}
