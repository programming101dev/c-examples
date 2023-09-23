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


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


static void setup_signal_handler(void);
static void sigint_handler(int signal_number);


int main(void)
{
    sigset_t block_set;
    pid_t pid;

    setup_signal_handler();

    // Block SIGINT temporarily
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    if(sigprocmask(SIG_BLOCK, &block_set, NULL) < 0)
    {
        perror("Failed to block SIGINT");
        return EXIT_FAILURE;
    }

    pid = fork();

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
        sigset_t empty_set;
        int status;

        // Parent process
        printf("Parent waiting for SIGINT...\n");

        // TODO: should I put something in the set?
        sigemptyset(&empty_set);
        sigsuspend(&empty_set);
        printf("Parent received SIGINT.\n");

        // Wait for the child process to complete
        waitpid(pid, &status, 0);
    }

    // Unblock SIGINT
    if(sigprocmask(SIG_UNBLOCK, &block_set, NULL) < 0)
    {
        perror("Failed to unblock SIGINT");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void setup_signal_handler(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    sa.sa_handler = sigint_handler;
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}


static void sigint_handler(int signal_number)
{
    printf("Received signal: %d\n", signal_number);
}
