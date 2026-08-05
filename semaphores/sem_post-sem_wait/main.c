/*
 * Copyright 2026 D'Arcy Smith
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    // TODO pass this in on the command line
    const char *sem_name = "/example_semaphore";
    sem_t      *semaphore;
    pid_t       pid;

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

        // Decrement the semaphore, block if not available
        if(sem_wait(semaphore) == -1)
        {
            perror("sem_wait failed");
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
        printf("Parent process (PID %d): Waiting for the child to complete...\n", getpid());

        // Wait for the semaphore signal from the child
        if(sem_wait(semaphore) == -1)
        {
            perror("sem_wait failed");
            sem_close(semaphore);
            sem_unlink(sem_name);
            exit(EXIT_FAILURE);
        }

        printf("Parent process (PID %d): Done sem_wait\n", getpid());

        // Release the semaphore (post)
        sem_post(semaphore);
        sem_close(semaphore);

        // Wait for the child process to finish
        if(wait(NULL) == -1)
        {
            perror("wait failed");
            sem_unlink(sem_name);
            exit(EXIT_FAILURE);
        }

        printf("Parent process (PID %d): Done waiting.\n", getpid());

        // Unlink the semaphore after use
        if(sem_unlink(sem_name) == -1)
        {
            perror("sem_unlink failed");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}
