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
    char *endptr;
    gid_t new_gid;

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <new_gid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    new_gid = (gid_t) strtol(argv[1], &endptr, 10);

    if(*endptr != '\0')
    {
        fprintf(stderr, "Invalid GID format: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if(setgid(new_gid) == -1)
    {
        perror("setgid");
        return EXIT_FAILURE;
    }

    printf("Real GID: %d\n", getgid());
    printf("Effective GID: %d\n", getegid());

    return EXIT_SUCCESS;
}
