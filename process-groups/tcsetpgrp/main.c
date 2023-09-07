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
#include <sys/wait.h>
#include <unistd.h>


int main(void)
{
    int terminal_fd = open("/dev/tty", O_RDWR); // Open the controlling terminal

    if(terminal_fd == -1)
    {
        perror("Error opening terminal");
        return 1;
    }

    pid_t new_pgid = getpid();
    printf("Original Process ID (PID): %d\n", new_pgid);

    // Fork a new child process
    pid_t child_pid = fork();

    if(child_pid < 0)
    {
        perror("Error forking a new process");
        close(terminal_fd);
        return 1;
    }
    else if(child_pid == 0)
    {
        // Child process
        new_pgid = getpid() + 100;

        printf("Child Process ID (PID): %d\n", getpid());

        if(setsid() == -1)
        {
            perror("Error creating a new session for the child process");
            close(terminal_fd);
            return 1;
        }

        printf("Child Process Group ID (PGID) after creating a new session: %d\n", getpid());

        // TODO: linux always fails?
        if(setpgid(getpid(), new_pgid) == -1)
        {
            perror("Error setting PGID for the child process");
            close(terminal_fd);
            return 1;
        }

        printf("Child Process Group ID (PGID) after change: %d\n", new_pgid);

        close(terminal_fd);

        return EXIT_SUCCESS;
    }
    else
    {
        // Parent process
        // Wait for the child process to finish
        wait(NULL);

        printf("Parent Process ID (PID) after child process execution: %d\n", getpid());

        close(terminal_fd);

        return EXIT_SUCCESS;
    }
}
