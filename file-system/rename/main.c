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

    return EXIT_SUCCESS;}


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
