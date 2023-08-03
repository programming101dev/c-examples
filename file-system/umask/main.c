#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


void createFile(const char *filename, const char *content);
void printFilePermissions(const char *filename);


int main(void)
{
    const char *filename1 = "file1.txt";
    const char *filename2 = "file2.txt";
    const char *content = "This is a sample file.\n";
    mode_t old_mask;
    mode_t new_mask;

    old_mask = umask(0);
    createFile(filename1, content);
    printf("File created with umask 000:\n");
    printFilePermissions(filename1);

    new_mask = umask(0222);
    createFile(filename2, content);
    printf("\nFile created with umask 222:\n");
    printFilePermissions(filename2);
    umask(old_mask);

    return EXIT_SUCCESS;
}


void createFile(const char *filename, const char *content)
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


void printFilePermissions(const char *filename)
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

