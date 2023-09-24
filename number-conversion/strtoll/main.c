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


#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void convert(const char *str);


#define BASE_TEN 10


int main(void)
{
    const char goodNumber[]     = "12345";
    const char leftoverNumber[] = "42hello";
    const char badNumber[]      = "abcde";
    convert(goodNumber);
    convert(leftoverNumber);
    convert(badNumber);
    return EXIT_SUCCESS;
}


static void convert(const char *str)
{
    long long int result;
    char          *endptr;
    errno  = 0;
    result = strtoll(str, &endptr, BASE_TEN);

    // Check for conversion errors
    if((errno == ERANGE && (result == LLONG_MAX || result == LLONG_MIN)) || (errno != 0 && result == 0))
    {
        fprintf(stderr, "Error during conversion: %s\n", strerror(errno));
    }

    // Check if the entire string was converted
    if(endptr == str)
    {
        fprintf(stderr, "No digits were found in the input.\n");
    }

    // Check for leftover characters in the string
    if(*endptr != '\0')
    {
        fprintf(stderr, "Extra characters after the number: %s\n", endptr);
    }
    printf("Result: %lld\n", result);
}
