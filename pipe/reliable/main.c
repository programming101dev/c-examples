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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **file_path);
static void handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void child_process(int pipefd[2], FILE *file, sem_t *sem_parent, sem_t *sem_child);
static void parent_process(int pipefd[2], sem_t *sem_parent, sem_t *sem_child);
static void send_word(int pipefd, const char *word, uint8_t length, sem_t *sem_parent, sem_t *sem_child);
_Noreturn static void error_exit(const char *msg);
static void write_fully(int fd, const void *buf, size_t count);
static void read_fully(int fd, void *buf, size_t count);


#define MAX_WORD_LENGTH 255
#define SEM_PARENT "/sem_parent"
#define SEM_CHILD "/sem_child"
#define UNKNOWN_OPTION_MESSAGE_LEN 24


int main(int argc, char *argv[])
{
    char  *file_path;
    int   pipefd[2];
    FILE  *file;
    pid_t pid;
    sem_t *sem_parent;
    sem_t *sem_child;

    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);
    file = fopen(file_path, "r");       // NOLINT(android-cloexec-fopen)

    if(file == NULL)
    {
        error_exit("Error opening file");
    }

    if(pipe(pipefd) == -1)  // NOLINT(android-cloexec-pipe)
    {
        error_exit("Error creating pipe");
    }

    sem_parent = sem_open(SEM_PARENT, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, 0);

    if(sem_parent == SEM_FAILED)
    {
        error_exit("Error creating/opening SEM_PARENT semaphore");
    }

    sem_child = sem_open(SEM_CHILD, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, 1);

    if(sem_child == SEM_FAILED)
    {
        error_exit("Error creating/opening SEM_CHILD semaphore");
    }

    pid = fork();   // NOLINT(android-cloexec-fopen)

    if(pid == -1)
    {
        error_exit("Error creating child process");
    }

    if(pid == 0)
    {
        child_process(pipefd, file, sem_parent, sem_child);
    }
    else
    {
        fclose(file);
        parent_process(pipefd, sem_parent, sem_child);
    }

    sem_unlink(SEM_PARENT);
    sem_unlink(SEM_CHILD);

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


static void write_fully(int fd, const void *buf, size_t count)
{
    const char *ptr = (const char *)buf;

    while(count > 0)
    {
        ssize_t written_bytes;

        written_bytes = write(fd, ptr, count);

        if(written_bytes < 0)
        {
            error_exit("Error writing fully to pipe");
        }

        ptr += written_bytes;
        count -= (size_t)written_bytes;
    }
}


static void read_fully(int fd, void *buf, size_t count)
{
    char *ptr;

    ptr = (char *)buf;

    while(count > 0)
    {
        ssize_t read_bytes;

        read_bytes = read(fd, ptr, count);

        if(read_bytes < 0)
        {
            error_exit("Error reading fully from pipe");
        }

        ptr += read_bytes;
        count -= (size_t)read_bytes;
    }
}


static void send_word(int pipefd, const char *word, uint8_t length, sem_t *sem_parent, sem_t *sem_child)
{
    printf("Child: sending word of length %u: %s\n", length, word);
    write_fully(pipefd, &length, sizeof(length));

    if(length > 0)
    {
        write_fully(pipefd, word, length);
    }

    // Tell parent it can read now
    if(sem_post(sem_parent) == -1)
    {
        error_exit("Semaphore post operation failed in child process");
    }

    // Wait for parent to finish reading
    if(sem_wait(sem_child) == -1)
    {
        error_exit("Semaphore wait operation failed in child process");
    }
}


_Noreturn static void error_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}


static void child_process(int pipefd[2], FILE *file, sem_t *sem_parent, sem_t *sem_child)
{
    int     ch;
    char    word[MAX_WORD_LENGTH];
    uint8_t length;

    length = 0;
    close(pipefd[0]);

    while((ch = fgetc(file)) != EOF)
    {
        if(ch == ' ' || ch == '\n' || ch == '\t')
        {
            if(length > 0)
            {
                word[length] = '\0';
                send_word(pipefd[1], word, length, sem_parent, sem_child);
                length = 0;
            }
        }
        else
        {
            if(length >= MAX_WORD_LENGTH)
            {
                error_exit("Encountered a word longer than the maximum allowed length");
            }
            word[length++] = (char)ch;
        }
    }

    if(length > 0)
    {
        word[length] = '\0';
        send_word(pipefd[1], word, length, sem_parent, sem_child);
    }

    send_word(pipefd[1], NULL, 0, sem_parent, sem_child);

    if(fclose(file) != 0)
    {
        error_exit("Error closing file");
    }

    if(close(pipefd[1]) != 0)
    {
        error_exit("Error closing pipe");
    }
}


static void parent_process(int pipefd[2], sem_t *sem_parent, sem_t *sem_child)
{
    uint8_t length;
    char    word[MAX_WORD_LENGTH];

    close(pipefd[1]);

    while(1)
    {
        // Wait for child to write
        if(sem_wait(sem_parent) == -1)
        {
            error_exit("Semaphore wait operation failed in parent process");
        }

        read_fully(pipefd[0], &length, sizeof(length));

        if(length == 0)
        {
            break;
        }

        read_fully(pipefd[0], word, length);
        word[length] = '\0';
        printf("Parent: received word of length %u: %s\n", length, word);

        // Tell child it can write again
        if(sem_post(sem_child) == -1)
        {
            error_exit("Semaphore post operation failed in parent process");
        }
    }

    if(close(pipefd[0]) != 0)
    {
        error_exit("Error closing pipe");
    }
}
