#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main(void)
{
    int terminal_fd = open("/dev/tty", O_RDWR); // Open the controlling terminal

    if(terminal_fd == -1) {
        perror("Error opening terminal");
        return 1;
    }

    pid_t foreground_pgid = tcgetpgrp(terminal_fd);

    if(foreground_pgid == -1) {
        perror("Error getting foreground PGID");
        close(terminal_fd);
        return 1;
    }

    printf("Foreground Process Group ID (PGID) of the terminal: %d\n", foreground_pgid);

    close(terminal_fd);

    return EXIT_SUCCESS;
}
