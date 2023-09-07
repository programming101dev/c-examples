/*
 * This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International license.
 *
 * Author: D'Arcy Smith (ds@programming101.dev)
 *
 * You are free to:
 *   - Share: Copy and redistribute the material in any medium or format.
 *   - Under the following terms:
 *       - Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
 *       - NonCommercial: You may not use the material for commercial purposes.
 *       - NoDerivatives: If you remix, transform, or build upon the material, you may not distribute the modified material.
 *
 * For more details, please refer to the full license text at:
 * https://creativecommons.org/licenses/by-nc-nd/4.0/
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void search_for(const char *needle, const char *haystack);


// TODO: pass in the string and the search on the command line


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
