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
