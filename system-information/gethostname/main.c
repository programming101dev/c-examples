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
    size_t max_hostname_length;
    long   length;
    char  *hostname;
    int    result;

    length = sysconf(_SC_HOST_NAME_MAX);

    if(length <= 0)
    {
        perror("sysconf");
        return EXIT_FAILURE;
    }

    max_hostname_length = (size_t)length;
    hostname            = (char *)malloc(max_hostname_length + 1);

    if(hostname == NULL)
    {
        perror("malloc");
        return EXIT_FAILURE;
    }

    result = gethostname(hostname, max_hostname_length);

    if(result == 0)
    {
        printf("Hostname: %s\n", hostname);
    }
    else
    {
        perror("gethostname");
        free(hostname);
        return EXIT_FAILURE;
    }

    free(hostname);

    return EXIT_SUCCESS;
}
