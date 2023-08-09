#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


#define SHM_SIZE 1024
#define CLIENT_SEM_NAME "/client_semaphore"
#define SERVER_SEM_NAME "/server_semaphore"


size_t get_page_size(void)
{
    long page_size = sysconf(_SC_PAGESIZE);

    if(page_size == -1)
    {
        perror("sysconf");
        exit(EXIT_FAILURE);
    }

    return (size_t)page_size;
}

int main(void)
{
    int shm_fd;
    char *shm_ptr;
    sem_t *client_sem, *server_sem;
    size_t page_size = get_page_size();
    size_t shm_size = (SHM_SIZE + page_size - 1) & ~(page_size - 1);

    // Open the shared memory
    const char* shm_name = "/my_shared_memory";

    // Open the shared memory
    shm_fd = shm_open(shm_name, O_RDWR, S_IRUSR | S_IWUSR);
    if(shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Map the shared memory into the process address space
    shm_ptr = (char*)mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Open the client semaphore
    client_sem = sem_open(CLIENT_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if(client_sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Open the server semaphore
    server_sem = sem_open(SERVER_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if(server_sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    while(1) {
        // Wait for the client to signal a word
        printf("Waiting for client_sem\n");
        sem_wait(client_sem);

        // Check if the process is done
        if(strcmp(shm_ptr, "") == 0) {
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

    return 0;
}