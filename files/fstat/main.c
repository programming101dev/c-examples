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


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


// TODO: take the file name in from the command line


int main(void)
{
    int fd;
    struct stat fileStat;

    fd = open("../../example.txt", O_RDONLY);

    if(fd == -1)
    {
        perror("Error opening file");
        return 1;
    }

    if(fstat(fd, &fileStat) == -1)
    {
        perror("Error getting file stats");
        close(fd);
        return EXIT_FAILURE;
    }

    // Display file information
    printf("File Information:\n");
    printf("File Size: %lld bytes\n", (long long) fileStat.st_size);
    printf("File Permissions: %o\n", fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
    printf("File inode: %lld\n", (long long) fileStat.st_ino);
    printf("Device ID: %lld\n", (long long) fileStat.st_dev);
    printf("Number of hard links: %lld\n", (long long) fileStat.st_nlink);
    printf("File Owner UID: %d\n", fileStat.st_uid);
    printf("File Group GID: %d\n", fileStat.st_gid);
    printf("Last access time: %ld\n", fileStat.st_atime);
    printf("Last modification time: %ld\n", fileStat.st_mtime);
    printf("Last status change time: %ld\n", fileStat.st_ctime);

    if(close(fd) == -1)
    {
        perror("Error closing file descriptor");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
