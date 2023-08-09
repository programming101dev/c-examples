#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

int main(void)
{
    int terminal_fd = STDIN_FILENO; // Use standard input (stdin) as the terminal

    pid_t sid = tcgetsid(terminal_fd);
    if(sid == -1) {
        perror("Error getting SID");
        return 1;
    }

    printf("Session ID (SID) of the foreground process group: %d\n", sid);

    return EXIT_SUCCESS;
}
