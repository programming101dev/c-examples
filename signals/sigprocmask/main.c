#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int signal_number);

int main(void)
{
    // Set up the signal handler
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGUSR1, &sa, NULL) < 0) {
        perror("Failed to set signal handler");
        return EXIT_FAILURE;
    }

    // Block SIGUSR1 signal
    sigset_t new_mask, old_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGUSR1);

    if(sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
        perror("Failed to block SIGUSR1 signal");
        return EXIT_FAILURE;
    }

    printf("SIGUSR1 signal is blocked. Sending SIGUSR1...\n");
    raise(SIGUSR1); // This will not be delivered immediately due to the signal being blocked

    // Wait for a while to demonstrate that SIGUSR1 is blocked
    sleep(3);

    // Unblock SIGUSR1 signal
    if(sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        perror("Failed to unblock SIGUSR1 signal");
        return EXIT_FAILURE;
    }

    printf("SIGUSR1 signal is unblocked. Sending SIGUSR1...\n");
    raise(SIGUSR1); // This will be delivered and handled immediately

    // Wait for a while to allow the signal handler to run
    sleep(3);

    return EXIT_SUCCESS;
}

void signal_handler(int signal_number) {
    printf("Received signal: %d\n", signal_number);
}
