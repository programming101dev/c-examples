#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#define SHM_SIZE 1024
#define CLIENT_SEM_NAME "/client_semaphore"
#define SERVER_SEM_NAME "/server_semaphore"

size_t get_page_size() {
    int mib[2];
    size_t page_size;
    mib[0] = CTL_HW;
    mib[1] = HW_PAGESIZE;
    size_t len = sizeof(page_size);
    sysctl(mib, 2, &page_size, &len, NULL, 0);
    return page_size;
}

int main() {
    int shm_fd;
    char *shm_ptr;
    sem_t *client_sem, *server_sem;
    size_t page_size = get_page_size();
    size_t shm_size = (SHM_SIZE + page_size - 1) & ~(page_size - 1);

    // Open the shared memory
    const char* shm_name = "/my_shared_memory";

    // Open the shared memory
    shm_fd = shm_open(shm_name, O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Map the shared memory into the process address space
    shm_ptr = (char*)mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Open the client semaphore
    client_sem = sem_open(CLIENT_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (client_sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Open the server semaphore
    server_sem = sem_open(SERVER_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (server_sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Open and read the file
    FILE *file = fopen("../../example.txt", "r");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file)) {
        char *word;
        char *saveptr;
        word = strtok_r(buffer, " \t\n", &saveptr);
        while (word != NULL) {
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

    return 0;
}
