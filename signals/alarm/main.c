#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>


static void alarm_handler(int signal_number);


volatile sig_atomic_t alarm_received = false;


int main(void)
{
    struct sigaction sa;

    // Set up signal handler for SIGALRM
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    // Set the alarm to trigger after 2 seconds
    alarm(2);

    printf("Waiting for the alarm...\n");

    // Wait until the alarm is received
    while(!(alarm_received))
    {
        // Put any other processing you want here.
    }

    printf("Exiting.\n");

    return EXIT_SUCCESS;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static void alarm_handler(int signal_number)
{
    const char *message = "Alarm received!\n";
    write(STDERR_FILENO, message, strlen(message));
    alarm_received = true;
}
#pragma GCC diagnostic pop
