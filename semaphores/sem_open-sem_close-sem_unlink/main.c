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

int main(void)
{
    // TODO pass this in on the command line
    const char *sem_name = "/example_semaphore";
    sem_t      *semaphore;

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
