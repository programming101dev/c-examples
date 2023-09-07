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


// TODO: pass the file in on the command line


int main(void)
{
    const char buffer[] = "This is an attempt to write.";
    FILE *file;

    file = fopen("../../example.txt", "r");

    if(file == NULL)
    {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }

    // Try to write to the file (this will fail in read mode)
    if(fputs(buffer, file) == EOF)
    {
        if(ferror(file))
        {
            perror("Error writing to the file");
        }
        else
        {
            printf("End of file or other write error\n");
        }
    }

    // Close the file
    fclose(file);

    return EXIT_SUCCESS;
}
