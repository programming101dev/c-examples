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


static void parse_arguments(int argc, char *argv[], char **file_path);
static void handle_arguments(const char *binary_name, const char *file_path);
static _Noreturn void usage(const char *program_name, int exit_code, const char *message);
static size_t get_page_size(void);


#define SHM_SIZE 1024
#define CLIENT_SEM_NAME "/client_semaphore"
#define SERVER_SEM_NAME "/server_semaphore"
#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BUFFER_LEN 100


int main(int argc, char *argv[])
{
    char       *file_path;
    int        shm_fd;
    char       *shm_ptr;
    sem_t      *client_sem;
    sem_t      *server_sem;
    FILE       *file;
    char       buffer[BUFFER_LEN];
    const char *shm_name = "/my_shared_memory";
    size_t     page_size;
    size_t     shm_size;

    page_size = get_page_size();
    shm_size  = (SHM_SIZE + page_size - 1) & ~(page_size - 1);
    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);

    // Open and read the file
    file = fopen(file_path, "re");

    if(!file)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Open the shared memory
    shm_fd = shm_open(shm_name, O_RDWR, S_IRUSR | S_IWUSR);

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

    while(fgets(buffer, sizeof(buffer), file))
    {
        char *word;
        char *saveptr;
        word = strtok_r(buffer, " \t\n", &saveptr);

        while(word != NULL)
        {
            if(strlen(word) >= SHM_SIZE)
            {
                fprintf(stderr, "Word too long for shared memory: %s\n", word);
                exit(EXIT_FAILURE);
            }

            // Copy the word into shared memory
            strcpy(shm_ptr, word);
            printf("Client is copying \"%s\" to shared memory\n", word);

            // Signal the server that a word is ready
            sem_post(client_sem);

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


static void parse_arguments(int argc, char *argv[], char **file_path)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch(opt)
        {
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
            }
            case '?':
            {
                char message[UNKNOWN_OPTION_MESSAGE_LEN];

                snprintf(message, sizeof(message), "Unknown option '-%c'.", optopt);
                usage(argv[0], EXIT_FAILURE, message);
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The group id is required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *file_path = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *file_path)
{
    if(file_path == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The file path is required.");
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <file path>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
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
