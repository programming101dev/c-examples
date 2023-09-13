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
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>


int main(void)
{
    const char *sem_name = "/example_semaphore";
    sem_t *semaphore;
    pid_t pid;

    // Create a named semaphore with initial value 1 (available)
    semaphore = sem_open(sem_name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);

    if(semaphore == SEM_FAILED)
    {
        perror("Semaphore creation failed");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();

    if(pid < 0)
    {
        perror("Fork failed");
        sem_close(semaphore);
        sem_unlink(sem_name);
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        // Child process (producer)
        printf("Child process (PID %d): Simulating some work...\n", getpid());

        // Try to decrement the semaphore without blocking
        if(sem_wait(semaphore) == -1)
        {
            perror("sem_trywait failed");
            sem_close(semaphore);
            sem_unlink(sem_name);
            exit(EXIT_FAILURE);
        }

        sleep(3);

        // Release the semaphore (post)
        printf("Child process (PID %d): Done work.\n", getpid());
        sem_post(semaphore);
        sem_close(semaphore);
    }
    else
    {
        sleep(1);
        // Parent process (consumer)
        printf("Parent process (PID %d): Waiting for the child to complete...\n", getpid());

        // Wait for the semaphore signal from the child with timeout
        int try_count = 5;
        while(try_count > 0)
        {
            int ret = sem_trywait(semaphore);

            if(ret == 0)
            {
                printf("Parent process (PID %d): Done waiting\n", getpid());

                // Release the semaphore (post)
                sem_post(semaphore);

                printf("Parent process (PID %d): Child signaled. Continue processing...\n", getpid());
                break;
            }
            else if(ret == -1)
            {
                if(try_count == 1)
                {
                    perror("sem_trywait failed");
                    sem_close(semaphore);
                    sem_unlink(sem_name);
                    exit(EXIT_FAILURE);
                }
                else
                {
                    printf("Parent process (PID %d): Child not ready. Retrying...\n", getpid());
                    try_count--;
                    sleep(1);
                }
            }
        }
        sem_close(semaphore);

        // Wait for the child process to finish
        if(wait(NULL) == -1)
        {
            perror("wait failed");
            sem_unlink(sem_name);
            exit(EXIT_FAILURE);
        }

        printf("Parent process (PID %d): Child completed.\n", getpid());

        // Unlink the semaphore after use
        if(sem_unlink(sem_name) == -1)
        {
            perror("sem_unlink failed");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}
