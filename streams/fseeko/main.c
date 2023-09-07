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


static void display_file(FILE *file, const char *message);


// TODO: pass file name on the command line


int main(void)
{
    FILE *file;

    file = fopen("../../example.txt", "r");

    if(file == NULL)
    {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }

    display_file(file, "File contents:\n\n");
    fseeko(file, 0L, SEEK_SET);
    display_file(file, "\n\nFile contents after SEEK_SET:\n\n");
    fseeko(file, -10L, SEEK_CUR);
    display_file(file, "\n\nFile contents after SEEK_CUR -10:\n\n");
    fclose(file);
    return EXIT_SUCCESS;
}


static void display_file(FILE *file, const char *message)
{
    char ch;

    fputs(message, stdout);

    while((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }
}
