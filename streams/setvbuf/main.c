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
#include <unistd.h>

int main(int argc, char *argv[])
{
    // Fully buffered - probably won't print because the buffer won't get full
    if(setvbuf(stdout, NULL, _IOFBF, 0) != 0)
    {
        perror("Error setting the vbuf to _IOFBF");
        return EXIT_FAILURE;
    }

    printf("This is part one... ");
    sleep(1);
    printf("and this is part two.\n");
    sleep(1);

    // Line buffered - probably won't print because the buffer won't get full, until it hits \n
    if(setvbuf(stdout, NULL, _IOLBF, 0) != 0)
    {
        perror("Error setting the vbuf to _IOLBF");
        return EXIT_FAILURE;
    }

    printf("This is part three... ");
    sleep(1);
    printf("and this is part four.\n");
    sleep(1);

    // Not buffered - prints out after each printf
    if(setvbuf(stdout, NULL, _IONBF, 0) != 0)
    {
        perror("Error setting the vbuf to _IONBF");
        return EXIT_FAILURE;
    }

    printf("This is part five... ");
    sleep(3);
    printf("and this is part six.\n");
    sleep(3);

    return EXIT_SUCCESS;
}
