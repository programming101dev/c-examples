#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


void signal_handler(int signal_number);


int main(void)
{
    pid_t pid = getpid();

    // Set up the signal handler
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGUSR1, &sa, NULL) < 0)
    {
        perror("Failed to set signal handler");
        return EXIT_FAILURE;
    }

    printf("My process ID is: %d\n", pid);
    printf("Sending SIGUSR1 signal to my own process...\n");
    kill(pid, SIGUSR1);

    // Loop forever, waiting for signals.
    while (1)
    {
    }

    return EXIT_SUCCESS;
}

void signal_handler(int signal_number)
{
    printf("Received signal: %d\n", signal_number);
}
