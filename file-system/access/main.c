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
#include <errno.h>


static void check_file_access(const char *filename, int mode, const char *access_msg);


int main(void)
{
    const char *filename = "../../example.txt";

    check_file_access(filename, R_OK, "readable");
    check_file_access(filename, W_OK, "writable");
    check_file_access(filename, X_OK, "executable");

    return EXIT_SUCCESS;
}


static void check_file_access(const char *filename, int mode, const char *access_msg)
{
    if(access(filename, mode) == 0)
    {
        printf("%s is %s.\n", filename, access_msg);
    }
    else
    {
        if(errno == EACCES)
        {
            printf("Access denied: %s is not %s.\n", filename, access_msg);
        }
        else
        {
            perror("Error accessing file");
        }

        exit(EXIT_FAILURE);
    }
}
