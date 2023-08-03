#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void convert(const char *str);


int main(void)
{
    const char goodNumber[] = "12345.6789";
    const char leftoverNumber[] = "42.8857hello";
    const char badNumber[] = "abcde";

    convert(goodNumber);
    convert(leftoverNumber);
    convert(badNumber);

    return 0;
}


static void convert(const char *str)
{
    double result;
    char *endptr;

    result = 0;
    errno  = 0;
    result = strtod(str, &endptr);

    // Check for conversion errors
    if((errno == ERANGE && (result == HUGE_VAL || result == -HUGE_VAL)) || (errno != 0 && result == 0))
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

    printf("Result: %f\n", result);
}
