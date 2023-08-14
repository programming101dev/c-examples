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
    const char *file_path;
    char *endptr;
    uid_t user_id;
    gid_t group_id;

    if(argc != 4)
    {
        fprintf(stderr, "Usage: %s <file_path> <user_id> <group_id>\n", argv[0]);

        return EXIT_FAILURE;
    }

    file_path = argv[1];
    user_id = strtol(argv[2], &endptr, 10);

    if(*endptr != '\0')
    {
        fprintf(stderr, "Invalid user_id format: %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    group_id = strtol(argv[3], &endptr, 10);

    if(*endptr != '\0')
    {
        fprintf(stderr, "Invalid group_id format: %s\n", argv[3]);

        return EXIT_FAILURE;
    }

    if(chown(file_path, user_id, group_id) == -1)
    {
        perror("chown");

        return EXIT_FAILURE;
    }

    printf("File ownership changed successfully.\n");

    return EXIT_SUCCESS;
}
