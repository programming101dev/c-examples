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
#include <sys/wait.h>
#include <unistd.h>


static void handle_child(void);


int main(void)
{
    pid_t pid;
    pid_t pgid;
    pid_t child_pid;
    pid  = getpid(); // Get the PID of the current process
    pgid = getpgid(pid);
    printf("Original Process ID (PID): %d\n", pid);
    printf("Original Process Group ID (PGID): %d\n", pgid);

    // Fork a new child process
    child_pid = fork();
    if(child_pid < 0)
    {
        perror("Error forking a new process");
        return EXIT_FAILURE;
    }

    if(child_pid == 0)
    {
        handle_child();
    }
    else
    {
        // Parent process
        // Wait for the child process to finish
        wait(NULL);
        printf("Parent Process ID (PID) after child process execution: %d\n", getpid());
    }

    return EXIT_SUCCESS;
}


static void handle_child(void)
{
    pid_t child_pid;
    pid_t child_pgid;
    pid_t new_pgid;
    pid_t sid;
    child_pid  = getpid();
    child_pgid = getpgid(child_pid);
    printf("Child Process ID (PID): %d\n", child_pid);
    printf("Child Process Group ID (PGID): %d\n", child_pgid);

    // Create a new session for the child process
    sid = setsid();
    if(sid == -1)
    {
        perror("Error creating a new session for the child process");
        exit(EXIT_FAILURE);
    }
    printf("Child Process New Session ID (SID): %d\n", sid);
    new_pgid = getpgid(child_pid);
    printf("Child Process New Process Group ID (PGID): %d\n", new_pgid);
}
