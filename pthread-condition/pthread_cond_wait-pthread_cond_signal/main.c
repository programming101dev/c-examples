#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>


static void *child_process(void *arg);
static void *parent_process(void *arg);
static void send_word(const char *word);


#define MAX_WORD_LENGTH 255


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int word_ready = 0;
char *shared_word = NULL;


int main(void)
{
    pthread_t child_thread, parent_thread;

    if(pthread_create(&child_thread, NULL, child_process, NULL) != 0)
    {
        perror("Error creating child thread");
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&parent_thread, NULL, parent_process, NULL) != 0)
    {
        perror("Error creating parent thread");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(child_thread, NULL) != 0)
    {
        perror("Error joining child thread");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(parent_thread, NULL) != 0)
    {
        perror("Error joining parent thread");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}


static void send_word(const char *word)
{
    pthread_mutex_lock(&mutex);

    while(word_ready)
    {
        if(pthread_cond_wait(&cond, &mutex) != 0)
        {
            perror("Error waiting for condition variable");
            exit(EXIT_FAILURE);
        }
    }

    shared_word = strdup(word);

    if(shared_word == NULL)
    {
        perror("Error duplicating word");
        exit(EXIT_FAILURE);
    }

    word_ready = 1;

    if(pthread_cond_signal(&cond) != 0)
    {
        perror("Error signaling condition variable");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_unlock(&mutex);
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static void *child_process(void *arg)
{
    FILE *file;
    char *token, *saveptr;
    char line[MAX_WORD_LENGTH];

    file = fopen("../../example.txt", "r");
    if(file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while(fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0'; // Remove the newline character if present

        token = strtok_r(line, " \t", &saveptr);
        while(token != NULL)
        {
            send_word(token);
            token = strtok_r(NULL, " \t", &saveptr);
        }
    }

    send_word(NULL); // Signal end of words

    if(fclose(file) != 0)
    {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    pthread_exit(NULL);
}
#pragma GCC diagnostic pop


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static void *parent_process(void *arg)
{
    char *word;

    while(1)
    {
        pthread_mutex_lock(&mutex);

        while(!word_ready)
        {
            if(pthread_cond_wait(&cond, &mutex) != 0)
            {
                perror("Error waiting for condition variable");
                exit(EXIT_FAILURE);
            }
        }

        word_ready = 0;

        if(pthread_cond_signal(&cond) != 0)
        {
            perror("Error signaling condition variable");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_unlock(&mutex);

        word = shared_word;

        if(word == NULL)
        {
            break;
        }

        printf("Parent: received word: %s\n", word);
        free(word); // Free the allocated memory for each word
    }

    pthread_exit(NULL);
}
#pragma GCC diagnostic pop
