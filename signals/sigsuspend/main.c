#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static void signal_handler(int signal_number);

int main(void) {
    sigset_t block_set;
    struct sigaction sa;

    // Set up signal handler for SIGINT
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) < 0) {
        perror("Failed to set signal handler for SIGINT");
        return 1;
    }

    // Block SIGINT temporarily
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    if (sigprocmask(SIG_BLOCK, &block_set, NULL) < 0) {
        perror("Failed to block SIGINT");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        // Child process
        sleep(3);
        printf("Child sending SIGINT to parent...\n");
        kill(getppid(), SIGINT);
        return 0;
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

    return 0;
}

static void signal_handler(int signal_number) {
    printf("Received signal: %d\n", signal_number);
}
