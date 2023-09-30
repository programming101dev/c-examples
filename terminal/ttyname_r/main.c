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
    long   length;
    size_t max_name_length;
    char  *ttyname_buf;

    length = sysconf(_SC_TTY_NAME_MAX);

    if(length == -1)
    {
        perror("Error getting maximum terminal name length");
        return 1;
    }

    max_name_length = (size_t)length;
    ttyname_buf     = (char *)malloc(max_name_length + 1);

    if(ttyname_buf == NULL)
    {
        perror("Error allocating memory for terminal name buffer");
        return EXIT_FAILURE;
    }

    if(ttyname_r(STDIN_FILENO, ttyname_buf, max_name_length + 1) == 0)
    {
        printf("The terminal associated with stdin is: %s\n", ttyname_buf);
    }
    else
    {
        perror("stdin is not associated with a terminal");
    }

    free(ttyname_buf);    // Free the allocated memory

    return EXIT_SUCCESS;
}
