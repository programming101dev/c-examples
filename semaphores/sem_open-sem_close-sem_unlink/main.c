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
