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


#include <getopt.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void *child_process(void *arg);
static void *parent_process(void *arg);
static void send_word(const char *word);


static void parse_arguments(int argc, char *argv[], char **file_path);
static void handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


#define MAX_WORD_LENGTH 255


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int word_ready = 0;
char *shared_word = NULL;


// TODO: this doesn't work properly - send_word(NULL) crashes


int main(int argc, char *argv[])
{
    char *file_path;
    pthread_t child_thread;
    pthread_t parent_thread;

    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);

    if(pthread_create(&child_thread, NULL, child_process, file_path) != 0)
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
                char message[24];

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
    const char *file_path;

    file_path = arg;

    file = fopen(file_path, "r");
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

    send_word(NULL);

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
