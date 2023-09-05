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


// TODO: take the path as a command line argument


int main(void)
{
    const char *filename = "../../example.txt";
    struct stat file_stat;

    if(stat(filename, &file_stat) == -1)
    {
        perror("Error getting file information");
        return EXIT_FAILURE;
    }

    printf("File information for: %s\n", filename);
    printf("File size: %lld bytes\n", (long long) file_stat.st_size);
    printf("File permissions: %o\n", file_stat.st_mode & 0777);

    return EXIT_SUCCESS;
}
