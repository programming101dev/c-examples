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
#include <termios.h>
#include <unistd.h>

int main(void)
{
    int   terminal_fd;
    pid_t sid;

    terminal_fd = STDIN_FILENO;    // Use standard input (stdin) as the terminal
    sid         = tcgetsid(terminal_fd);

    if(sid == -1)
    {
        perror("Error getting SID");
        return 1;
    }

    printf("Session ID (SID) of the foreground process group: %d\n", sid);

    return EXIT_SUCCESS;
}
