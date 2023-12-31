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
    // TODO pass on the command line
    char tempdir_template[] = "exampleXXXXXX";

    if(mkdtemp(tempdir_template) == NULL)
    {
        perror("Error creating temporary directory");
        return EXIT_FAILURE;
    }

    printf("Temporary directory created: %s\n", tempdir_template);

    if(rmdir(tempdir_template) == -1)
    {
        perror("Error removing temporary directory");
        return EXIT_FAILURE;
    }

    printf("Temporary directory removed: %s\n", tempdir_template);

    return EXIT_SUCCESS;
}
