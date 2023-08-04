#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    // Get the current process ID
    pid_t pid = getpid();

    // Get the current priority of the process
    int current_priority = nice(0);

    printf("Process ID: %d\n", pid);
    printf("Current Priority: %d\n", current_priority);

    // Try to increase the priority by 10 (less favorable)
    int new_priority = nice(10);

    // Get the new priority after the change
    int updated_priority = nice(0);

    printf("New Priority: %d\n", new_priority);
    printf("Updated Priority: %d\n", updated_priority);

    return EXIT_SUCCESS;
}
