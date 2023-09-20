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


static void setup_signal_handler(void);
static void sigint_handler(int signal_number);
static int check_pending_signal(void);
static int block_signal(int signal_num, sigset_t *block_set);
static int unblock_signal(sigset_t *block_set);


int main(void)
{
    sigset_t block_set;

    setup_signal_handler();

    // Check for pending signals before blocking
    printf("SIGINT is %s before blocking.\n", check_pending_signal() ? "pending" : "not pending");

    // Block SIGINT temporarily
    if(block_signal(SIGINT, &block_set) != 0)
    {
        return EXIT_FAILURE;
    }

    printf("Sending SIGINT\n");
    raise(SIGINT);

    // Check for pending signals after raising
    printf("SIGINT is %s and blocked.\n", check_pending_signal() ? "pending" : "not pending");

    printf("Unblocking SIGINT\n");

    // Unblock SIGINT
    if(unblock_signal(&block_set) != 0)
    {
        return 1;
    }

    // Check for pending signals after unblocking
    printf("SIGINT is %s after unblocking.\n", check_pending_signal() ? "pending" : "not pending");

    return EXIT_SUCCESS;
}

static void setup_signal_handler(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
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

static int check_pending_signal(void)
{
    sigset_t pending_set;
    if(sigpending(&pending_set) != 0)
    {
        perror("Failed to get pending signals");
        return EXIT_FAILURE;
    }

    return sigismember(&pending_set, SIGINT);
}

static int block_signal(int signal_num, sigset_t *block_set)
{
    sigemptyset(block_set);
    sigaddset(block_set, signal_num);

    if(sigprocmask(SIG_BLOCK, block_set, NULL) < 0)
    {
        perror("Failed to block signal");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static int unblock_signal(sigset_t *block_set)
{
    if(sigprocmask(SIG_UNBLOCK, block_set, NULL) < 0)
    {
        perror("Failed to unblock signal");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
