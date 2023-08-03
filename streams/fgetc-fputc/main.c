#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    int c;

    printf("Please input some text, end with Ctrl-D:\n");

    while((c = fgetc(stdin)) != EOF)
    {
        putchar(c);
    }

    return EXIT_SUCCESS;
}
