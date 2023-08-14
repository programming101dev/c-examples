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


void signal_handler(int signal_number);


struct signal_info
{
    int signal_number;
    const char *signal_name;
};


int main(void)
{
    struct signal_info signals[] =
    {
        {SIGABRT,   "SIGABRT"},
        {SIGALRM,   "SIGALRM"},
        {SIGCHLD,   "SIGCHLD"},
        {SIGCONT,   "SIGCONT"},
        {SIGFPE,    "SIGFPE"},
        {SIGHUP,    "SIGHUP"},
        {SIGILL,    "SIGILL"},
        {SIGINT,    "SIGINT"},
        {SIGPIPE,   "SIGPIPE"},
        {SIGQUIT,   "SIGQUIT"},
        {SIGSEGV,   "SIGSEGV"},
        {SIGSYS,    "SIGSYS"},
        {SIGTERM,   "SIGTERM"},
        {SIGTRAP,   "SIGTRAP"},
        {SIGTSTP,   "SIGTSTP"},
        {SIGTTIN,   "SIGTTIN"},
        {SIGTTOU,   "SIGTTOU"},
        {SIGURG,    "SIGURG"},
        {SIGUSR1,   "SIGUSR1"},
        {SIGUSR2,   "SIGUSR2"},
        {SIGVTALRM, "SIGVTALRM"},
        {SIGXCPU,   "SIGXCPU"},
        {SIGXFSZ,   "SIGXFSZ"}
    };

    int n = sizeof(signals) / sizeof(signals[0]);
    struct sigaction sa;
    struct sigaction new_sa;
    pid_t pid = getpid();

    new_sa.sa_handler = signal_handler;
    sigemptyset(&new_sa.sa_mask);
    new_sa.sa_flags = 0;

    for(int i = 0; i < n; i++)
    {
        if(sigaction(signals[i].signal_number, NULL, &sa) == 0)
        {
            if(sa.sa_handler == SIG_IGN)
            {
                printf("Signal %s (%d) is currently being ignored\n", signals[i].signal_name, signals[i].signal_number);
            }
            else if(sa.sa_handler == SIG_DFL)
            {
                printf("Signal %s (%d) is set to default action\n", signals[i].signal_name, signals[i].signal_number);
            }
            else
            {
                printf("Signal %s (%d) is currently caught by a signal handler\n", signals[i].signal_name,
                       signals[i].signal_number);
            }
        }
        else
        {
            printf("Failed to retrieve signal info: %s (%d)\n", signals[i].signal_name, signals[i].signal_number);
        }

        if(sigaction(signals[i].signal_number, &new_sa, NULL) < 0)
        {
            printf("Failed to set signal handler: %s (%d)\n", signals[i].signal_name, signals[i].signal_number);
        }
    }

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


void signal_handler(int signal_number)
{
    printf("Received signal: %d\n", signal_number);
}
