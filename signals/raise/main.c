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
#include <unistd.h>

static void signal_handler(int signal_number);

int main(void)
{
    const int signals_to_handle[] = {SIGABRT, SIGALRM, SIGCHLD, SIGCONT, SIGFPE, SIGHUP, SIGILL, SIGINT, SIGKILL, SIGPIPE, SIGQUIT, SIGSEGV, SIGSTOP, SIGSYS, SIGTERM, SIGTRAP, SIGTSTP, SIGTTIN, SIGTTOU, SIGURG, SIGUSR1, SIGUSR2, SIGVTALRM, SIGXCPU, SIGXFSZ};
    pid_t     pid;
    struct sigaction sa;

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    sa.sa_handler = signal_handler;
#if defined(__clang__)
    #pragma clang diagnostic pop
#endif

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    for(size_t i = 0; i < sizeof(signals_to_handle) / sizeof(signals_to_handle[0]); i++)
    {
        if(sigaction(signals_to_handle[i], &sa, NULL) < 0)
        {
            printf("Failed to set signal handler for signal %d\n", signals_to_handle[i]);
        }
        else
        {
            printf("Set signal handler for signal %d\n", signals_to_handle[i]);
            raise(signals_to_handle[i]);
        }
    }

    pid = getpid();
    printf("My process ID is: %d\n", pid);
    printf("Sending SIGUSR1 signal to my own process...\n");
    raise(SIGUSR1);
    printf("To send a signal:\n");    // cppcheck-suppress unreachableCode
    printf("\tCtrl+C: Sends the SIGINT signal to the process.\n");
    printf("\tCtrl+Z: Sends the SIGTSTP signal to the process.\n");
    printf("\tCtrl+\\: Sends the SIGQUIT signal to the process.\n");
    printf("Or start a new terminal\n");
    printf("To end the process use:\n");
    printf("\tkill -KILL %d      # SIGKILL\n", pid);
    printf("To pull the process into the background use:\n");
    printf("\tkill -STOP %d      # SIGSTOP\n", pid);

    // Loop forever, waiting for signals.
    while(1)
    {
    }

    return EXIT_SUCCESS;
}

static void signal_handler(int signal_number)
{
    printf("Received signal: %d\n", signal_number);
}
