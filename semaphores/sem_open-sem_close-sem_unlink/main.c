#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    const char *sem_name = "/example_semaphore";
    sem_t *semaphore;

    // Create a named semaphore with initial value 1 (available)
    printf("Opening %s\n", sem_name);
    semaphore = sem_open(sem_name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);

    if(semaphore == SEM_FAILED)
    {
        perror("Semaphore creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Closing %s\n", sem_name);

    // Close the semaphore after use
    if(sem_close(semaphore) == -1)
    {
        perror("sem_close failed");
        exit(EXIT_FAILURE);
    }

    printf("Unlinking %s\n", sem_name);

    // Unlink the semaphore after use
    if(sem_unlink(sem_name) == -1)
    {
        perror("sem_unlink failed");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
