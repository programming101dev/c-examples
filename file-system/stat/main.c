#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


int main(void)
{
    const char *filename = "../../example.txt";
    struct stat file_stat;

    if(stat(filename, &file_stat) == -1)
    {
        perror("Error getting file information");
        return EXIT_FAILURE;
    }

    printf("File information for: %s\n", filename);
    printf("File size: %lld bytes\n", (long long)file_stat.st_size);
    printf("File permissions: %o\n", file_stat.st_mode & 0777);

    return EXIT_SUCCESS;
}