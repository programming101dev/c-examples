#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


void display_file(int fd, const char *message);


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
    display_file(fd, "\n\nFile contents after SEEK_CUR -10:\n\n");

    close(fd);
    return EXIT_SUCCESS;
}


void display_file(int fd, const char *message)
{
    char ch;

    write(STDOUT_FILENO, message, strlen(message));

    while(read(fd, &ch, 1) > 0)
    {
        write(STDOUT_FILENO, &ch, 1);
    }
}
