#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main(void)
{
    int terminal_fd = open("/dev/tty", O_RDWR); // Open the controlling terminal

    if(terminal_fd == -1)
    {
        perror("Error opening terminal");
        return 1;
    }

    pid_t new_pgid = getpid(); // Use the PID of the current process as the new PGID

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
        new_pgid = getpid() + 100; // Set a different value for the new PGID

        printf("Child Process ID (PID): %d\n", getpid());

        // Create a new session for the child process
        if(setsid() == -1)
        {
            perror("Error creating a new session for the child process");
            close(terminal_fd);
            return 1;
        }

        printf("Child Process Group ID (PGID) after creating a new session: %d\n", getpid());

        // Change the PGID of the child process
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
