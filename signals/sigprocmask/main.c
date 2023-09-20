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
#include <unistd.h>
#include <signal.h>


static void setup_signal_handler(void);
static void signal_handler(int signal_number);


int main(void)
{
    // Set up the signal handler
    struct sigaction sa;
    sigset_t new_mask, old_mask;

    setup_signal_handler();

    // Block SIGUSR1 signal
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGUSR1);

    if(sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0)
    {
        perror("Failed to block SIGUSR1 signal");
        return EXIT_FAILURE;
    }

    printf("SIGUSR1 signal is blocked. Sending SIGUSR1...\n");
    raise(SIGUSR1); // This will not be delivered immediately due to the signal being blocked

    // Wait for a while to demonstrate that SIGUSR1 is blocked
    sleep(3);

    // Unblock SIGUSR1 signal
    if(sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
    {
        perror("Failed to unblock SIGUSR1 signal");
        return EXIT_FAILURE;
    }

    printf("SIGUSR1 signal is unblocked. Sending SIGUSR1...\n");
    raise(SIGUSR1); // This will be delivered and handled immediately

    // Wait for a while to allow the signal handler to run
    sleep(3);

    return EXIT_SUCCESS;
}


static void setup_signal_handler(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}


static void signal_handler(int signal_number)
{
    printf("Received signal: %d\n", signal_number);
}
