#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

volatile sig_atomic_t exit_flag = 0;

void alarm_handler(int signal_number) {
    printf("Alarm received! Signal: %d\n", signal_number);

    static int num_alarms = 0;
    num_alarms++;

    if (num_alarms >= 3) {
        printf("Exiting after 3 alarms.\n");
        exit_flag = 1;
    }
}

int main(void) {
    struct sigaction sa;
    struct sigevent sev;
    timer_t timerid;
    struct itimerspec its;

    // Set up signal handler for SIGALRM
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    // Create a timer that will send SIGALRM
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) < 0) {
        perror("Failed to create timer");
        return 1;
    }

    // Set the timer to trigger after 1.5 seconds and repeat every 1.5 seconds
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 500000000; // 1.5 seconds
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 500000000; // 1.5 seconds
    if (timer_settime(timerid, 0, &its, NULL) < 0) {
        perror("Failed to set timer");
        return 1;
    }

    printf("Waiting for alarms...\n");

    // Loop to wait for signals (will exit after 3 alarms)
    while (!exit_flag) {
        // Do other work here if needed
    }

    // Clean up the timer
    timer_delete(timerid);

    return 0;
}
