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


#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>


// TODO: take the path as a command line argument


int main(void)
{
    DIR *dir = opendir(".");

    if(dir == NULL)
    {
        perror("opendir");
        return EXIT_FAILURE;
    }

    // Do something with the directory...

    if(closedir(dir) == -1)
    {
        perror("closedir");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
