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
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


static void signal_handler(int signal_number);


int main(void)
{
    sigset_t block_set;
    struct sigaction sa;

    // Set up signal handler for SIGINT
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGINT, &sa, NULL) < 0)
    {
        perror("Failed to set signal handler for SIGINT");
        return 1;
    }

    // Block SIGINT temporarily
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    if(sigprocmask(SIG_BLOCK, &block_set, NULL) < 0)
    {
        perror("Failed to block SIGINT");
        return 1;
    }

    pid_t pid = fork();

    if(pid < 0)
    {
        perror("Fork failed");
        return 1;
    }
    else if(pid == 0)
    {
        // Child process
        sleep(3);
        printf("Child sending SIGINT to parent...\n");
        kill(getppid(), SIGINT);
        return EXIT_SUCCESS;
    }
    else
    {
        // Parent process
        printf("Parent waiting for SIGINT...\n");
        sigset_t empty_set;
        sigemptyset(&empty_set);
        sigsuspend(&empty_set);
        printf("Parent received SIGINT.\n");

        // Wait for the child process to complete
        int status;
        waitpid(pid, &status, 0);
    }

    // Unblock SIGINT
    if(sigprocmask(SIG_UNBLOCK, &block_set, NULL) < 0)
    {
        perror("Failed to unblock SIGINT");
        return 1;
    }

    return EXIT_SUCCESS;
}

static void signal_handler(int signal_number)
{
    printf("Received signal: %d\n", signal_number);
}
