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


static void exit_handler1(void);
static void exit_handler2(void);
static void exit_handler3(void);


int main(void)
{
    atexit(exit_handler1);
    atexit(exit_handler2);
    atexit(exit_handler3);
    printf("Main function executing.\n");
    exit(EXIT_SUCCESS);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunreachable-code"
    printf("This won't be seen.\n");

    return EXIT_FAILURE;
#pragma GCC diagnostic pop
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
