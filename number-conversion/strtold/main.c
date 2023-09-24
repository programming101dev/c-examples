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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void convert(const char *str);


#define TOLLERANCE 1e-6L


int main(void)
{
    const char goodNumber[]     = "12345.6789";
    const char leftoverNumber[] = "42.99989hello";
    const char badNumber[]      = "abcde";
    convert(goodNumber);
    convert(leftoverNumber);
    convert(badNumber);
    return EXIT_SUCCESS;
}


static void convert(const char *str)
{
    long double result;
    char        *endptr;
    long double tolerance;
    errno     = 0;
    result    = strtold(str, &endptr);

    // Define your desired tolerance
    tolerance = TOLLERANCE;

    // Check for conversion errors
    if((errno == ERANGE && (fabsl(result - HUGE_VALL) < tolerance)) || (errno != 0 && fabsl(result) < tolerance))
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
    printf("Result: %Lf\n", result);
}
