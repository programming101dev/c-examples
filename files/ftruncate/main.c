#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


static void check_file_size(const char *filename, const char *message);


int main(void)
{
    const char *content = "This is a sample file.\n";
    char filename[] = "example_XXXXXX";
    int fd;
    off_t new_size;

    fd = mkstemp(filename);

    if(fd == -1)
    {
        perror("Error creating temporary file");
        return EXIT_FAILURE;
    }

    dprintf(fd, "%s\n", content);
    check_file_size(filename, "Initial file size");
    new_size = 9;
    printf("Truncating file %s to %lld bytes\n", filename, (long long) new_size);

    if(ftruncate(fd, new_size) == -1)
    {
        perror("Error truncating file");
        close(fd);
        return EXIT_FAILURE;
    }

    if(close(fd) == -1)
    {
        perror("Error closing file");
        return EXIT_FAILURE;
    }

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
        printf("%s of '%s' is %lld bytes.\n", message, filename, (long long) st.st_size);
    }
    else
    {
        perror("Error getting file size");
    }
}
