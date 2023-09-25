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


static void create_sample_file(const char *filename);
static void create_hard_link(const char *source_filename, const char *link_filename);
static void check_file_existence(const char *filename, const char *message);


int main(void)
{
    // TODO pass the filenames in on the command line
    const char *source_filename = "example.txt";
    const char *link_filename   = "hard_link.txt";

    printf("Creating file %s\n", source_filename);
    create_sample_file(source_filename);
    check_file_existence(source_filename, "Source file");
    check_file_existence(link_filename, "Link file");
    printf("Creating hard link %s\n", link_filename);
    create_hard_link(source_filename, link_filename);
    check_file_existence(source_filename, "Source file");
    check_file_existence(link_filename, "Hard link");
    printf("Deleting file %s\n", source_filename);

    if(unlink(source_filename) == -1)
    {
        perror("Error deleting source file");
        return EXIT_FAILURE;
    }

    check_file_existence(source_filename, "Source file");
    check_file_existence(link_filename, "Hard link");
    printf("Deleting hard link %s\n", link_filename);

    if(unlink(link_filename) == -1)
    {
        perror("Error deleting hard link");
        return EXIT_FAILURE;
    }

    check_file_existence(source_filename, "Source file");
    check_file_existence(link_filename, "Hard link");

    return EXIT_SUCCESS;
}


static void create_sample_file(const char *filename)
{
    FILE *file;

    file = fopen(filename, "we");

    if(file != NULL)
    {
        fprintf(file, "This is a sample file.\n");
        fclose(file);
    }
}


static void create_hard_link(const char *source_filename, const char *link_filename)
{
    if(link(source_filename, link_filename) == -1)
    {
        perror("Error creating hard link");
        exit(EXIT_FAILURE);
    }
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
