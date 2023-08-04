#include <stdio.h>
#include <signal.h>


static void print_signal_set(const sigset_t *signal_set);


int main(void)
{
    // Create an empty signal set
    sigset_t signal_set;

    if(sigemptyset(&signal_set) != 0)
    {
        perror("Failed to create an empty signal set");
        return 1;
    }

    printf("Empty signal set:\n");
    print_signal_set(&signal_set);

    // Check if a specific signal is in the set
    int signal_to_check = SIGINT;

    if(sigismember(&signal_set, signal_to_check))
    {
        printf("Signal %d is in the set.\n", signal_to_check);
    }
    else
    {
        printf("Signal %d is not in the set.\n", signal_to_check);
    }

    // Add a signal to the set
    int signal_to_add = SIGINT;

    if(sigaddset(&signal_set, signal_to_add) != 0)
    {
        perror("Failed to add signal to the set");
        return 1;
    }

    printf("Signal set with %d:\n", signal_to_add);
    print_signal_set(&signal_set);

    // Check if the added signal is now in the set
    if (sigismember(&signal_set, signal_to_add))
    {
        printf("Signal %d is now in the set.\n", signal_to_add);
    }
    else
    {
        printf("Signal %d is still not in the set.\n", signal_to_add);
    }

    // Remove a signal from the set
    int signal_to_remove = SIGINT;

    if (sigdelset(&signal_set, signal_to_remove) != 0)
    {
        perror("Failed to remove signal from the set");
        return 1;
    }

    printf("Signal set without %d:\n", signal_to_remove);
    print_signal_set(&signal_set);

    // Check if the removed signal is still in the set
    if (sigismember(&signal_set, signal_to_remove))
    {
        printf("Signal %d is still in the set.\n", signal_to_remove);
    }
    else
    {
        printf("Signal %d has been removed from the set.\n", signal_to_remove);
    }

    // Fill the entire set with all available signals
    if(sigfillset(&signal_set) != 0)
    {
        perror("Failed to fill the signal set");
        return 1;
    }

    // Print the complete set
    printf("Full signal set:\n");
    print_signal_set(&signal_set);

    return 0;
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