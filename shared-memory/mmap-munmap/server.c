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
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


static size_t get_page_size(void);


#define SHM_SIZE 1024
#define CLIENT_SEM_NAME "/client_semaphore"
#define SERVER_SEM_NAME "/server_semaphore"


int main(void)
{
    int        shm_fd;
    char       *shm_ptr;
    sem_t      *client_sem;
    sem_t      *server_sem;
    size_t     page_size;
    size_t     shm_size;
    // TODO pass this in on the command line
    const char *shm_name = "/my_shared_memory";

    page_size = get_page_size();
    shm_size  = (SHM_SIZE + page_size - 1) & ~(page_size - 1);

    // Open the shared memory
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if(shm_fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Map the shared memory into the process address space
    shm_ptr = (char *)mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

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

    while(1)
    {
        // Wait for the client to signal a word
        printf("Waiting for client_sem\n");
        sem_wait(client_sem);

        if(shm_ptr == NULL)
        {
            sem_post(server_sem);
            break;
        }

        // Check if the process is done
        if(strcmp(shm_ptr, "") == 0)
        {
            // Signal the client that the server has finished processing
            sem_post(server_sem);
            break;
        }

        // Print the word
        printf("%s\n", shm_ptr);

        // Signal the client to proceed to the next word
        printf("Posting to server_sem\n");
        sem_post(server_sem);
    }

    // Wait for the client to indicate it has finished
    sem_wait(server_sem);

    // Cleanup
    sem_close(client_sem);
    sem_close(server_sem);
    munmap(shm_ptr, shm_size);
    close(shm_fd);

    return EXIT_SUCCESS;
}


static size_t get_page_size(void)
{
    long page_size;

    page_size = sysconf(_SC_PAGESIZE);

    if(page_size == -1)
    {
        perror("sysconf");
        exit(EXIT_FAILURE);
    }

    return (size_t)page_size;
}
