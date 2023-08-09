#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


static void handle_child(void);


int main(void)
{
    pid_t pid;
    pid_t pgid;
    pid_t child_pid;

    pid = getpid();
    pgid = getpgid(pid);
    printf("Original Process ID (PID): %d\n", pid);
    printf("Original Process Group ID (PGID): %d\n", pgid);

    // Fork a new child process
    child_pid = fork();

    if(child_pid < 0)
    {
        perror("Error forking a new process");
        return 1;
    }
    else if(child_pid == 0)
    {
        handle_child();
        return EXIT_SUCCESS;
    }
    else
    {
        // Parent process
        // Wait for the child process to finish
        wait(NULL);

        return EXIT_SUCCESS;
    }
}


static void handle_child(void)
{
    // Child process
    pid_t child_pid;
    pid_t child_pgid;

    child_pid = getpid();
    child_pgid = getpgid(child_pid);
    printf("Child Process ID (PID): %d\n", child_pid);
    printf("Child Process Group ID (PGID): %d\n", child_pgid);

    // Change the PGID of the child process
    if(setpgid(child_pid, child_pid) == -1)
    {
        perror("Error setting PGID for the child process");
        exit(EXIT_FAILURE);
    }

    child_pgid = getpgid(child_pid);
    printf("Child Process Group ID (PGID) after change: %d\n", child_pgid);
}
