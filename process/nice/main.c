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
