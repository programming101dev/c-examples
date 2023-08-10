#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(void)
{
    printf("Starting program...\n");
    printf("Sleeping for 3 seconds...\n");
    sleep(3);
    printf("Program woke up!\n");

    return EXIT_SUCCESS;
}
