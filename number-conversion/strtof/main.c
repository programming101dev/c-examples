#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void convert(const char *str);


int main(void)
{
    const char goodNumber[] = "123.456";
    const char leftoverNumber[] = "42.14234hello";
    const char badNumber[] = "abcde";

    convert(goodNumber);
    convert(leftoverNumber);
    convert(badNumber);

    return EXIT_SUCCESS;
}


static void convert(const char *str)
{
    float result;
    char *endptr;

    result = 0;
    errno  = 0;
    result = strtof(str, &endptr);

    // Check for conversion errors
    float tolerance = 1e-6f; // Define your desired tolerance for float
    if ((errno == ERANGE && (fabsf(result - HUGE_VALF) < tolerance || fabsf(result + HUGE_VALF) < tolerance)) || (errno != 0 && fabsf(result) < tolerance))
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

    printf("Result: %f\n", (double)result);
}
