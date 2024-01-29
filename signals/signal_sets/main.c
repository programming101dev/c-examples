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

static void print_signal_set(const sigset_t *signal_set);

int main(void)
{
    // Create an empty signal set
    sigset_t signal_set;
    int      signal_to_check;
    int      signal_to_add;
    int      signal_to_remove;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
    if((sigemptyset(&signal_set) != 0))
#pragma GCC diagnostic pop
    {
#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma GCC diagnostic   ignored "-Wunreachable-code"
#endif
        perror("Failed to create an empty signal set");
#if defined(__clang__)
    #pragma GCC diagnostic pop
#endif
        return EXIT_FAILURE;
    }

    printf("Empty signal set:\n");
    print_signal_set(&signal_set);

    // Check if a specific signal is in the set
    signal_to_check = SIGINT;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
    if((sigismember(&signal_set, signal_to_check)))
#pragma GCC diagnostic pop
    {
        printf("Signal %d is in the set.\n", signal_to_check);
    }
    else
    {
        printf("Signal %d is not in the set.\n", signal_to_check);
    }

    // Add a signal to the set
    signal_to_add = SIGINT;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
    if((sigaddset(&signal_set, signal_to_add) != 0))
#pragma GCC diagnostic pop
    {
#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma GCC diagnostic   ignored "-Wunreachable-code"
#endif
        perror("Failed to add signal to the set");
#if defined(__clang__)
    #pragma clang diagnostic pop
#endif
        return EXIT_FAILURE;
    }

    printf("Signal set with %d:\n", signal_to_add);
    print_signal_set(&signal_set);

    // Check if the added signal is now in the set
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
    if((sigismember(&signal_set, signal_to_add)))
#pragma GCC diagnostic pop
    {
        printf("Signal %d is now in the set.\n", signal_to_add);
    }
    else
    {
        printf("Signal %d is still not in the set.\n", signal_to_add);
    }

    // Remove a signal from the set
    signal_to_remove = SIGINT;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
    if((sigdelset(&signal_set, signal_to_remove) != 0))
#pragma GCC diagnostic pop
    {
#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma GCC diagnostic   ignored "-Wunreachable-code"
#endif
        perror("Failed to remove signal from the set");
#if defined(__clang__)
    #pragma clang diagnostic pop
#endif
        return EXIT_FAILURE;
    }

    printf("Signal set without %d:\n", signal_to_remove);
    print_signal_set(&signal_set);

    // Check if the removed signal is still in the set
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
    if((sigismember(&signal_set, signal_to_remove)))
#pragma GCC diagnostic pop
    {
        printf("Signal %d is still in the set.\n", signal_to_remove);
    }
    else
    {
        printf("Signal %d has been removed from the set.\n", signal_to_remove);
    }

    // Fill the entire set with all available signals
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
    if((sigfillset(&signal_set) != 0))
#pragma GCC diagnostic pop
    {
#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma GCC diagnostic   ignored "-Wunreachable-code"
#endif
        perror("Failed to fill the signal set");
#if defined(__clang__)
    #pragma clang diagnostic pop
#endif
        return EXIT_FAILURE;
    }

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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
        if((sigismember(signal_set, sig)))
#pragma GCC diagnostic pop
        {
            printf("\t- Signal %d is in the set.\n", sig);
        }
    }

    printf("-----\n");
}
