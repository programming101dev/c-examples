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
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


static void display_file(int fd, const char *message);


// TODO: take the file name & seek offset in from the command line


int main(void)
{
    int fd;

    fd = open("../../example.txt", O_RDONLY);

    if(fd == -1)
    {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }

    display_file(fd, "File contents:\n\n");
    lseek(fd, 0L, SEEK_SET);
    display_file(fd, "\n\nFile contents after SEEK_SET:\n\n");
    lseek(fd, -10L, SEEK_CUR);
    display_file(fd, "\n\nFile contents after SEEK_CUR - 10:\n\n");

    close(fd);
    return EXIT_SUCCESS;
}


static void display_file(int fd, const char *message)
{
    char ch;

    write(STDOUT_FILENO, message, strlen(message));

    while(read(fd, &ch, 1) > 0)
    {
        write(STDOUT_FILENO, &ch, 1);
    }
}
