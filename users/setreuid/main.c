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
    uid_t new_uid;
    uid_t new_euid;

    if(argc != 3)
    {
        fprintf(stderr, "Usage: %s <new_gid> <new_egid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    new_uid = (uid_t) strtol(argv[1], &endptr, 10);

    if(*endptr != '\0')
    {
        fprintf(stderr, "Invalid UID format: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    new_euid = (uid_t) strtol(argv[2], &endptr, 10);

    if(*endptr != '\0')
    {
        fprintf(stderr, "Invalid Effective UID format: %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    if(setreuid(new_uid, new_euid) == -1)
    {
        perror("setreuid");
        return EXIT_FAILURE;
    }

    printf("Real UID: %d\n", getuid());
    printf("Effective UID: %d\n", geteuid());

    return EXIT_SUCCESS;
}
