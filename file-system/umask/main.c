#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


static void create_file(const char *filename, const char *content);
static void print_file_permissions(const char *filename);
static void delete_file(const char *filename);


int main(void)
{
    const char *filename1 = "file1.txt";
    const char *filename2 = "file2.txt";
    const char *content = "This is a sample file.\n";
    mode_t old_mask;

    old_mask = umask(0);
    create_file(filename1, content);
    printf("File created with umask 000:\n");
    print_file_permissions(filename1);

    umask(0222);
    create_file(filename2, content);
    printf("\nFile created with umask 222:\n");
    print_file_permissions(filename2);
    umask(old_mask);

    delete_file(filename1);
    delete_file(filename2);

    return EXIT_SUCCESS;
}


static void create_file(const char *filename, const char *content)
{
    FILE *file;

    file = fopen(filename, "w");

    if(file == NULL)
    {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }

    if(fprintf(file, "%s", content) < 0)
    {
        perror("Error writing to file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}


static void print_file_permissions(const char *filename)
{
    struct stat fileStat;

    if(stat(filename, &fileStat) == -1)
    {
        perror("Error getting file stats");
        return;
    }

    printf("File: %s\n", filename);
    printf("Permissions: %o\n", fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
}


static void delete_file(const char *filename)
{
    if(unlink(filename) == -1)
    {
        perror("Error deleting file");
        exit(EXIT_FAILURE);
    }
}
