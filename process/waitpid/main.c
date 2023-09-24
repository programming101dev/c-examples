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


static void print_process_info(const char *name);


int main(void)
{
    pid_t pid;
    pid = fork();
    if(pid == -1)
    {
        perror("Error creating child process");
        return EXIT_FAILURE;
    }
    if(pid == 0)
    {
        // This is the child process
        print_process_info("Child");
        printf("Child process finished.\n");
    }
    else
    {
        int status;

        // This is the parent process
        print_process_info("Parent");
        printf("Parent is waiting for the child\n");

        // Wait for the child process to finish
        if(waitpid(pid, &status, 0) == -1)
        {
            perror("Error waiting for child process");
            return EXIT_FAILURE;
        }
        printf("Parent process finished.\n");
    }
    return EXIT_SUCCESS;
}


static void print_process_info(const char *name)
{
    printf("Name: %s, Process: PID=%d, Parent PID=%d\n", name, getpid(), getppid());
}
