#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>


static void check_file_size(const char *filename, const char *message);


int main(void)
{
    const char *filename = "example.txt";
    off_t new_size;
    FILE *file;

    file = fopen(filename, "w");

    if(file != NULL)
    {
        fprintf(file, "This is a sample file.\n");
        fclose(file);
    }

    check_file_size(filename, "Truncated file size");
    new_size = 9;
    printf("Truncating file %s to %lld bytes\n", filename, (long long) new_size);

    if(truncate(filename, new_size) == -1)
    {
        perror("Error truncating file");
        return EXIT_FAILURE;
    }

    check_file_size(filename, "Truncated file size");

    if(unlink(filename) == -1)
    {
        perror("Error unlinking file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


static void check_file_size(const char *filename, const char *message)
{
    struct stat st;
    if(stat(filename, &st) == 0)
    {
        printf("\t%s of '%s' is %lld bytes.\n", message, filename, (long long) st.st_size);
    }
    else
    {
        perror("Error getting file size");
    }
}
