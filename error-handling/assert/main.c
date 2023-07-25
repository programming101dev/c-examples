#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    int x;
    int y;

    x = 10;
    y = 20;

    // If the condition is false, the program will terminate with an error message.
    // If the condition is true, the program continues normally.
    assert(x < y);
    assert(x > y);

    return EXIT_SUCCESS;
}
