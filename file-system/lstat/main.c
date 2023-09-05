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
#include <sys/stat.h>
#include <unistd.h>


// TODO: pass the filename in the command line args, maybe use a default
int main(void)
{
    const char *filename = "../../example.txt";
    const char *symlink_name = "symlink_example.txt";
    struct stat link_stat;
    struct stat target_stat;

    if(symlink(filename, symlink_name) == -1)
    {
        perror("Error creating symbolic link");
        return EXIT_FAILURE;
    }

    if(lstat(symlink_name, &link_stat) == -1)
    {
        perror("Error getting link information");
        return EXIT_FAILURE;
    }

    if(stat(symlink_name, &target_stat) == -1)
    {
        perror("Error getting target file information");
        return EXIT_FAILURE;
    }

    printf("Symbolic link information for: %s\n", symlink_name);
    printf("Link size: %lld bytes\n", (long long) link_stat.st_size);
    printf("Link permissions: %o\n", link_stat.st_mode & 0777);

    printf("\nTarget file information (follows the symbolic link):\n");
    printf("Target size: %lld bytes\n", (long long) target_stat.st_size);
    printf("Target permissions: %o\n", target_stat.st_mode & 0777);

    // Remove the symbolic link
    if(unlink(symlink_name) == -1)
    {
        perror("Error unlinking symbolic link");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
