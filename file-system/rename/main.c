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


static void check_file_existence(const char *filename, const char *message);


int main(void)
{
    const char *old_filename = "old_file.txt";
    const char *new_filename = "new_file.txt";
    FILE *file;

    file = fopen(old_filename, "w");

    if(file != NULL)
    {
        fprintf(file, "This is a sample file.\n");
        fclose(file);
    }

    printf("Creating file %s\n", old_filename);
    check_file_existence(old_filename, "Original file");
    check_file_existence(new_filename, "Renamed file");

    printf("Renaming file %s to %s\n", old_filename, new_filename);

    if(rename(old_filename, new_filename) == -1)
    {
        perror("Error renaming file");
        return EXIT_FAILURE;
    }

    check_file_existence(old_filename, "Original file");
    check_file_existence(new_filename, "Renamed file");

    printf("Removing file %s\n", new_filename);

    if(unlink(new_filename) == -1)
    {
        perror("Error unlinking file");
        return EXIT_FAILURE;
    }

    check_file_existence(old_filename, "Original file");
    check_file_existence(new_filename, "Renamed file");

    return EXIT_SUCCESS;
}


static void check_file_existence(const char *filename, const char *message)
{
    if(access(filename, F_OK) == 0)
    {
        printf("\t%s '%s' exists.\n", message, filename);
    }
    else
    {
        printf("\t%s '%s' does not exist.\n", message, filename);
    }
}
