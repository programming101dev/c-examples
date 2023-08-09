#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    int age;
    double height;

    age = 30;
    height = 1.75;
    fprintf(stdout, "My age is %d and my height is %.2f meters.\n", age, height);
    fprintf(stderr, "Error: Invalid input. Expected an integer, but got '%s'.\n", "xyz");

    return EXIT_SUCCESS;
}
