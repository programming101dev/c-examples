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
#include <time.h>
#include <unistd.h>

static void print_process_info(const char *name);
static void child_process(const char *name);

#define MAX_SECONDS 5

int main(void)
{
    pid_t pid1;
    pid_t pid2;
    int   status1;
    int   status2;

    pid1 = fork();

    if(pid1 == -1)
    {
        perror("Error creating child process 1");
        return EXIT_FAILURE;
    }

    if(pid1 == 0)
    {
        // This is the first child process
        child_process("Child 1");
        return EXIT_SUCCESS;
    }

    pid2 = fork();

    if(pid2 == -1)
    {
        perror("Error creating child process 2");
        return EXIT_FAILURE;
    }

    if(pid2 == 0)
    {
        // This is the second child process
        child_process("Child 2");
        return EXIT_SUCCESS;
    }

    // This is the parent process
    print_process_info("Parent");

    // Wait for the child processes to finish
    if(waitpid(pid1, &status1, 0) == -1)
    {
        perror("Error waiting for child process 1");
        return EXIT_FAILURE;
    }

    if(waitpid(pid2, &status2, 0) == -1)
    {
        perror("Error waiting for child process 2");
        return EXIT_FAILURE;
    }

    // Print exit status of child processes
    if(WIFEXITED(status1))
    {
        printf("Child 1 exited with status: %d\n", WEXITSTATUS(status1));
    }

    if(WIFEXITED(status2))
    {
        printf("Child 2 exited with status: %d\n", WEXITSTATUS(status2));
    }

    return EXIT_SUCCESS;
}

static void print_process_info(const char *name)
{
    printf("Name: %s, Process: PID=%d, Parent PID=%d\n", name, getpid(), getppid());
}

static void child_process(const char *name)
{
    unsigned int sleep_time;

    srand((unsigned int)time(NULL) ^ (unsigned int)getpid());
    print_process_info(name);
    sleep_time = (unsigned int)rand() % MAX_SECONDS;
    sleep(sleep_time);
    printf("%s process finished after sleeping %u seconds.\n", name, sleep_time);
}
