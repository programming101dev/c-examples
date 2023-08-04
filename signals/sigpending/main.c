#include <stdio.h>
#include <signal.h>


static void signal_handler(int signal_number);
static int check_pending_signal();
static int block_signal(int signal_num, sigset_t* block_set);
static int unblock_signal(sigset_t* block_set);


int main(void)
{
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

    // Check for pending signals before blocking
    printf("SIGINT is %s before blocking.\n", check_pending_signal() ? "pending" : "not pending");

    // Block SIGINT temporarily
    if (block_signal(SIGINT, &block_set) != 0) {
        return 1;
    }

    printf("Sending SIGINT\n");
    raise(SIGINT);

    // Check for pending signals after raising
    printf("SIGINT is %s and blocked.\n", check_pending_signal() ? "pending" : "not pending");

    printf("Unblocking SIGINT\n");

    // Unblock SIGINT
    if (unblock_signal(&block_set) != 0) {
        return 1;
    }

    // Check for pending signals after unblocking
    printf("SIGINT is %s after unblocking.\n", check_pending_signal() ? "pending" : "not pending");

    return 0;
}

static void signal_handler(int signal_number) {
    printf("Received signal: %d\n", signal_number);
}

static int check_pending_signal() {
    sigset_t pending_set;
    if (sigpending(&pending_set) != 0) {
        perror("Failed to get pending signals");
        return 1;
    }

    return sigismember(&pending_set, SIGINT);
}

static int block_signal(int signal_num, sigset_t* block_set) {
    sigemptyset(block_set);
    sigaddset(block_set, signal_num);

    if (sigprocmask(SIG_BLOCK, block_set, NULL) < 0) {
        perror("Failed to block signal");
        return 1;
    }

    return 0;
}

static int unblock_signal(sigset_t* block_set)
{
    if (sigprocmask(SIG_UNBLOCK, block_set, NULL) < 0) {
        perror("Failed to unblock signal");
        return 1;
    }

    return 0;
}
