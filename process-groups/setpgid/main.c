#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid(); // Get the PID of the current process
    pid_t pgid = getpgid(pid);

    printf("Original Process ID (PID): %d\n", pid);
    printf("Original Process Group ID (PGID): %d\n", pgid);

    // Fork a new child process
    pid_t child_pid = fork();

    if (child_pid < 0)
    {
        perror("Error forking a new process");
        return 1;
    }
    else if (child_pid == 0)
    {
        // Child process
        pid_t child_pid = getpid();
        pid_t child_pgid = getpgid(child_pid);

        printf("Child Process ID (PID): %d\n", child_pid);
        printf("Child Process Group ID (PGID): %d\n", child_pgid);

        // Change the PGID of the child process
        if (setpgid(child_pid, child_pid) == -1) {
            perror("Error setting PGID for the child process");
            return 1;
        }

        child_pgid = getpgid(child_pid);
        printf("Child Process Group ID (PGID) after change: %d\n", child_pgid);

        return 0;
    }
    else
    {
        // Parent process
        // Wait for the child process to finish
        wait(NULL);

        return 0;
    }
}
