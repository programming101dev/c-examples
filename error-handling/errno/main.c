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


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    errno = 0;
    fgetc(stdout);
    if(errno != 0)
    {
        int current_errno;
        current_errno = errno;
        errno         = 0;
        if(fprintf(stderr, "errno: %d\n", current_errno) < 0)
        {
            printf("could not fprintf: %d\n", errno);
        }
    }
    return EXIT_SUCCESS;
}
