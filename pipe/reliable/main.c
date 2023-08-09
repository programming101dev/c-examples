#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>


#define MAX_WORD_LENGTH 255
#define SEM_PARENT "/sem_parent"
#define SEM_CHILD "/sem_child"


// Function Prototypes
int main(void);
void child_process(int pipefd[2], sem_t *sem_parent, sem_t *sem_child);
void parent_process(int pipefd[2], sem_t *sem_parent, sem_t *sem_child);
void send_word(int pipefd, const char *word, uint8_t length, sem_t *sem_parent, sem_t *sem_child);
void error_exit(const char *msg);
void write_fully(int fd, const void *buf, size_t count);
void read_fully(int fd, void *buf, size_t count);

void write_fully(int fd, const void *buf, size_t count)
{
    const char *ptr = buf;
    while(count > 0)
    {
        ssize_t written_bytes = write(fd, ptr, count);
        if(written_bytes < 0)
        {
            error_exit("Error writing fully to pipe");
        }
        ptr += written_bytes;
        count -= written_bytes;
    }
}

void read_fully(int fd, void *buf, size_t count)
{
    char *ptr = buf;
    while(count > 0)
    {
        ssize_t read_bytes = read(fd, ptr, count);
        if(read_bytes < 0)
        {
            error_exit("Error reading fully from pipe");
        }
        ptr += read_bytes;
        count -= read_bytes;
    }
}

void send_word(int pipefd, const char *word, uint8_t length, sem_t *sem_parent, sem_t *sem_child)
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

void error_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void child_process(int pipefd[2], sem_t *sem_parent, sem_t *sem_child)
{
    FILE *file;
    char ch;
    char word[MAX_WORD_LENGTH];
    uint8_t length = 0;

    close(pipefd[0]);

    file = fopen("../../example.txt", "r");
    if(file == NULL)
    {
        error_exit("Error opening file");
    }

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
            word[length++] = ch;
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

    exit(EXIT_SUCCESS);
}

void parent_process(int pipefd[2], sem_t *sem_parent, sem_t *sem_child)
{
    uint8_t length;
    char word[MAX_WORD_LENGTH];

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

    exit(EXIT_SUCCESS);
}

int main(void)
{
    int pipefd[2];
    pid_t pid;
    sem_t *sem_parent, *sem_child;

    if(pipe(pipefd) == -1)
    {
        error_exit("Error creating pipe");
    }

    sem_parent = sem_open(SEM_PARENT, O_CREAT, 0644, 0);
    if(sem_parent == SEM_FAILED)
    {
        error_exit("Error creating/opening SEM_PARENT semaphore");
    }

    sem_child = sem_open(SEM_CHILD, O_CREAT, 0644, 1);
    if(sem_child == SEM_FAILED)
    {
        error_exit("Error creating/opening SEM_CHILD semaphore");
    }

    pid = fork();
    if(pid == -1)
    {
        error_exit("Error creating child process");
    }

    if(pid == 0)
    {
        child_process(pipefd, sem_parent, sem_child);
    }
    else
    {
        parent_process(pipefd, sem_parent, sem_child);
    }

    sem_unlink(SEM_PARENT);
    sem_unlink(SEM_CHILD);

    return EXIT_SUCCESS;  // This line will not be executed, but it's here to keep the compiler happy.
}