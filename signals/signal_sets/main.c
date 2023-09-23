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
#include <signal.h>


static void print_signal_set(const sigset_t *signal_set);


int main(void)
{
    // Create an empty signal set
    sigset_t signal_set;
    int      signal_to_check;
    int      signal_to_add;
    int      signal_to_remove;
#ifdef __APPLE__
    sigemptyset(&signal_set);  // On macOS, this call is guaranteed to succeed
#else
    if(sigemptyset(&signal_set) != 0)
    {
        perror("Failed to create an empty signal set");
        return EXIT_FAILURE;
    }
#endif
    printf("Empty signal set:\n");
    print_signal_set(&signal_set);

    // Check if a specific signal is in the set
    signal_to_check = SIGINT;
    if(sigismember(&signal_set, signal_to_check))
    {
        printf("Signal %d is in the set.\n", signal_to_check);
    }
    else
    {
        printf("Signal %d is not in the set.\n", signal_to_check);
    }

    // Add a signal to the set
    signal_to_add = SIGINT;
#ifdef __APPLE__
    sigaddset(&signal_set, signal_to_add); // On macOS, this call is guaranteed to succeed
#else
    if(sigaddset(&signal_set, signal_to_add) != 0)
    {
        perror("Failed to add signal to the set");
        return EXIT_FAILURE;
    }
#endif
    printf("Signal set with %d:\n", signal_to_add);
    print_signal_set(&signal_set);

    // Check if the added signal is now in the set
    if(sigismember(&signal_set, signal_to_add))
    {
        printf("Signal %d is now in the set.\n", signal_to_add);
    }
    else
    {
        printf("Signal %d is still not in the set.\n", signal_to_add);
    }

    // Remove a signal from the set
    signal_to_remove = SIGINT;
#ifdef __APPLE__
    sigdelset(&signal_set, signal_to_remove); // On macOS, this call is guaranteed to succeed
#else
    if(sigdelset(&signal_set, signal_to_remove) != 0)
    {
        perror("Failed to remove signal from the set");
        return EXIT_FAILURE;
    }
#endif
    printf("Signal set without %d:\n", signal_to_remove);
    print_signal_set(&signal_set);

    // Check if the removed signal is still in the set
    if(sigismember(&signal_set, signal_to_remove))
    {
        printf("Signal %d is still in the set.\n", signal_to_remove);
    }
    else
    {
        printf("Signal %d has been removed from the set.\n", signal_to_remove);
    }

    // Fill the entire set with all available signals
#if defined(__APPLE__)
    sigfillset(&signal_set);  // Always succeeds on macOS, so no need to check
#else
    if(sigfillset(&signal_set) != 0)
    {
        perror("Failed to fill the signal set");
        return EXIT_FAILURE;
    }
#endif

    // Print the complete set
    printf("Full signal set:\n");
    print_signal_set(&signal_set);
    return EXIT_SUCCESS;
}


static void print_signal_set(const sigset_t *signal_set)
{
    printf("-----\n");
    for(int sig = 1; sig <= NSIG; sig++)
    {
        if(sigismember(signal_set, sig))
        {
            printf("\t- Signal %d is in the set.\n", sig);
        }
    }
    printf("-----\n");
}
