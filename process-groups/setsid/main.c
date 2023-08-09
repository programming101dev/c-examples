#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = getpid(); // Get the PID of the current process
    pid_t pgid = getpgid(pid);

    printf("Original Process ID (PID): %d\n", pid);
    printf("Original Process Group ID (PGID): %d\n", pgid);

    // Fork a new child process
    pid_t child_pid = fork();

    if(child_pid < 0) {
        perror("Error forking a new process");
        return 1;
    } else if(child_pid == 0) {
        // Child process
        pid_t child_pid = getpid();
        pid_t child_pgid = getpgid(child_pid);

        printf("Child Process ID (PID): %d\n", child_pid);
        printf("Child Process Group ID (PGID): %d\n", child_pgid);

        // Create a new session for the child process
        pid_t sid = setsid();
        if(sid == -1) {
            perror("Error creating a new session for the child process");
            return 1;
        }

        printf("Child Process New Session ID (SID): %d\n", sid);

        pid_t new_pgid = getpgid(child_pid);
        printf("Child Process New Process Group ID (PGID): %d\n", new_pgid);

        return EXIT_SUCCESS;
    } else {
        // Parent process
        // Wait for the child process to finish
        wait(NULL);

        printf("Parent Process ID (PID) after child process execution: %d\n", getpid());

        return EXIT_SUCCESS;
    }
}
