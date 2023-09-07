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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>


static size_t get_page_size(void);


#define SHM_SIZE 1024
#define CLIENT_SEM_NAME "/client_semaphore"
#define SERVER_SEM_NAME "/server_semaphore"


// TODO: pass file name on command line


int main(void)
{
    int shm_fd;
    char *shm_ptr;
    sem_t *client_sem, *server_sem;
    size_t page_size = get_page_size();
    size_t shm_size = (SHM_SIZE + page_size - 1) & ~(page_size - 1);

    // Open the shared memory
    const char *shm_name = "/my_shared_memory";

    // Open the shared memory
    shm_fd = shm_open(shm_name, O_RDWR, S_IRUSR | S_IWUSR);
    if(shm_fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Map the shared memory into the process address space
    shm_ptr = (char *) mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Open the client semaphore
    client_sem = sem_open(CLIENT_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if(client_sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Open the server semaphore
    server_sem = sem_open(SERVER_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if(server_sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Open and read the file
    FILE *file = fopen("../../example.txt", "r");
    if(!file)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char buffer[100];

    while(fgets(buffer, sizeof(buffer), file))
    {
        char *word;
        char *saveptr;
        word = strtok_r(buffer, " \t\n", &saveptr);

        while(word != NULL)
        {
            // TODO: this crashes on Linux
            // Copy the word into shared memory
            strcpy(shm_ptr, word);

            printf("Posting to client_sem\n");
            // Signal the server that a word is ready
            sem_post(client_sem);

            printf("Waiting for server_sem\n");
            // Wait for the server to acknowledge
            sem_wait(server_sem);

            // Read the next word
            word = strtok_r(NULL, " \t\n", &saveptr);
        }
    }

    // Signal the server that the client is done
    strcpy(shm_ptr, "");
    sem_post(client_sem);

    // Wait for the server to acknowledge that it has finished processing
    sem_wait(server_sem);

    // Cleanup
    fclose(file);
    sem_close(client_sem);
    sem_close(server_sem);
    munmap(shm_ptr, shm_size);
    close(shm_fd);

    return EXIT_SUCCESS;
}


static size_t get_page_size(void)
{
    long page_size = sysconf(_SC_PAGESIZE);

    if(page_size == -1)
    {
        perror("sysconf");
        exit(EXIT_FAILURE);
    }

    return (size_t) page_size;
}
