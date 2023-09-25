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
#include <unistd.h>


int main(void)
{
    pid_t foreground_pgid;
    int   terminal_fd;

    terminal_fd = open("/dev/tty", O_RDWR | O_CLOEXEC); // Open the controlling terminal

    if(terminal_fd == -1)
    {
        perror("Error opening terminal");
        return 1;
    }

    foreground_pgid = tcgetpgrp(terminal_fd);

    if(foreground_pgid == -1)
    {
        perror("Error getting foreground PGID");
        close(terminal_fd);
        return 1;
    }

    printf("Foreground Process Group ID (PGID) of the terminal: %d\n", foreground_pgid);
    close(terminal_fd);

    return EXIT_SUCCESS;
}
