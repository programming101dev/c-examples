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


int main(void)
{
    // Fully buffered - probably won't print because the buffer won't get full
    setvbuf(stdout, NULL, _IOFBF, 0);

    printf("This is part one... ");
    sleep(2);

    // flush the stream to force the output
    fflush(stdout);

    printf("and this is part two.\n");
    sleep(2);

    // no fflush needed - it happens when the program exits.

    return EXIT_SUCCESS;
}
