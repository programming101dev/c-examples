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
#include <string.h>


int main(void)
{
    errno = 0;
    fgetc(stdout);
    if(errno != 0)
    {
        const char *msg;
        int        current_erro_no;
        current_erro_no = errno;
        errno           = 0;
        msg             = strerror(current_erro_no);
        if(errno != 0)
        {
            perror("strerror");
            return EXIT_FAILURE;
        }
        errno = 0;
        if(fprintf(stderr, "There was an error with fgetc: %s\n", msg) < 0)
        {
            perror("fprintf");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
