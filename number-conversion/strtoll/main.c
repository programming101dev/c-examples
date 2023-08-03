#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void convert(const char *str);


int main(void)
{
    const char goodNumber[] = "12345";
    const char leftoverNumber[] = "42hello";
    const char badNumber[] = "abcde";

    convert(goodNumber);
    convert(leftoverNumber);
    convert(badNumber);

    return 0;
}


static void convert(const char *str)
{
    long long int result;
    char *endptr;

    result = 0;
    errno  = 0;
    result = strtoll(str, &endptr, 10);

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
