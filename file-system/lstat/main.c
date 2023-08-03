#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


int main(void)
{
    const char *filename = "example.txt";
    const char *symlink_name = "symlink_example.txt";
    struct stat link_stat;
    struct stat target_stat;

    if (symlink("../../example.txt", symlink_name) == -1)
    {
        perror("Error creating symbolic link");
        return EXIT_FAILURE;
    }

    if(lstat(symlink_name, &link_stat) == -1)
    {
        perror("Error getting link information");
        return EXIT_FAILURE;
    }

    if(stat(symlink_name, &target_stat) == -1)
    {
        perror("Error getting target file information");
        return EXIT_FAILURE;
    }

    printf("Symbolic link information for: %s\n", symlink_name);
    printf("Link size: %lld bytes\n", (long long)link_stat.st_size);
    printf("Link permissions: %o\n", link_stat.st_mode & 0777);

    printf("\nTarget file information (follows the symbolic link):\n");
    printf("Target size: %lld bytes\n", (long long)target_stat.st_size);
    printf("Target permissions: %o\n", target_stat.st_mode & 0777);

    // Remove the symbolic link
    if (unlink(symlink_name) == -1)
    {
        perror("Error unlinking symbolic link");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
