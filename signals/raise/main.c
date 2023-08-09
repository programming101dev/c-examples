#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void signal_handler(int signal_number);


int main(void)
{
    // Set up the signal handler
    pid_t pid;
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGUSR1, &sa, NULL) < 0)
    {
        perror("Failed to set signal handler");
        return EXIT_FAILURE;
    }

    pid = getpid();
    printf("Sending SIGUSR1 signal to my own process...\n");
    raise(SIGUSR1);
    printf("To send a signal:\n");
    printf("\tCtrl+C: Sends the SIGINT signal to terminate the process.\n");
    printf("Or start a new terminal and type:\n");
    printf("kill -ABRT %d       // SIGABRT\n", pid);
    printf("kill -ALRM %d       // SIGALRM\n", pid);
    printf("kill -CHLD %d       // SIGCHLD\n", pid);
    printf("kill -CONT %d       // SIGCONT\n", pid);
    printf("kill -FPE %d        // SIGFPE\n", pid);
    printf("kill -HUP %d        // SIGHUP\n", pid);
    printf("kill -ILL %d        // SIGILL\n", pid);
    printf("kill -INT %d        // SIGINT\n", pid);
    printf("kill -KILL %d       // SIGKILL\n", pid);
    printf("kill -PIPE %d       // SIGPIPE\n", pid);
    printf("kill -QUIT %d       // SIGQUIT\n", pid);
    printf("kill -SEGV %d       // SIGSEGV\n", pid);
    printf("kill -STOP %d       // SIGSTOP\n", pid);
    printf("kill -SYS %d        // SIGSYS\n", pid);
    printf("kill -TERM %d       // SIGTERM\n", pid);
    printf("kill -TRAP %d       // SIGTRAP\n", pid);
    printf("kill -TSTP %d       // SIGTSTP\n", pid);
    printf("kill -TTIN %d       // SIGTTIN\n", pid);
    printf("kill -TTOU %d       // SIGTTOU\n", pid);
    printf("kill -URG %d        // SIGURG\n", pid);
    printf("kill -USR1 %d       // SIGUSR1\n", pid);
    printf("kill -USR2 %d       // SIGUSR2\n", pid);
    printf("kill -VTALRM %d     // SIGVTALRM\n", pid);
    printf("kill -XCPU %d       // SIGXCPU\n", pid);
    printf("kill -XFSZ %d       // SIGXFSZ\n", pid);

    // Loop forever, waiting for signals.
    while(1)
    {
    }

    return EXIT_SUCCESS;
}

void signal_handler(int signal_number) {
    printf("Received signal: %d\n", signal_number);
}
