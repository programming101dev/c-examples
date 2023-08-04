#include <stdio.h>
#include <stdlib.h>


static void exit_handler1(void);
static void exit_handler2(void);
static void exit_handler3(void);


int main(void)
{
    atexit(exit_handler1);
    atexit(exit_handler2);
    atexit(exit_handler3);
    printf("Main function executing.\n");

    return EXIT_SUCCESS;
}


static void exit_handler1(void)
{
    printf("exit_handler1 called.\n");
}


static void exit_handler2(void)
{
    printf("exit_handler2 called.\n");
}


static void exit_handler3(void)
{
    printf("exit_handler3 called.\n");
}
