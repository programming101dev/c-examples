#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Signal handler function (empty, not used in this example)
void signal_handler(int signal_number) {}

int main(void)
{
    sigset_t mask;
    int sig;
    struct sigaction sa;

    // Set up signal handler for SIGINT (empty, not used in this example)
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Create a signal mask containing SIGINT
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    // Block SIGINT in the main thread
    if(sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
        perror("Failed to block SIGINT");
        return 1;
    }

    // Fork the process
    pid_t pid = fork();
    if(pid < 0) {
        perror("Fork failed");
        return 1;
    } else if(pid == 0) {
        // Child process
        sleep(3); // Sleep for 3 seconds
        kill(getppid(), SIGINT); // Send SIGINT to the parent
        exit(0);
    } else {
        // Parent process
        printf("Blocked SIGINT. Waiting for SIGINT using sigwait...\n");

        // Wait for SIGINT using sigwait
        if(sigwait(&mask, &sig) != 0) {
            perror("Failed to wait for signal");
            return 1;
        }

        // Print the received signal number
        printf("Received signal: %d\n", sig);

        // Wait for the child process to complete
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
