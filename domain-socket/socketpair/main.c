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


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>


#define MAX_WORD_LENGTH 255


void child_process(int sockfd);

void parent_process(int sockfd);

void send_word(int sockfd, const char *word, uint8_t length);

void error_exit(const char *msg);


int main(void)
{
    int sockfd[2];

    if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == -1)
    {
        error_exit("Error creating socket pair");
    }

    pid_t pid = fork();

    if(pid == -1)
    {
        error_exit("Error creating child process");
    }

    if(pid == 0)
    {
        close(sockfd[1]);
        child_process(sockfd[0]);
    }
    else
    {
        close(sockfd[0]);
        parent_process(sockfd[1]);
    }

    return EXIT_SUCCESS;
}


void send_word(int sockfd, const char *word, uint8_t length)
{
    ssize_t written_bytes;

    printf("Child: sending word of length %u: %s\n", length, word);
    written_bytes = write(sockfd, &length, sizeof(length));

    if(written_bytes < 0)
    {
        error_exit("Error writing word length to socket");
    }

    if(length > 0)
    {
        written_bytes = write(sockfd, word, length);

        if(written_bytes < 0)
        {
            error_exit("Error writing word to socket");
        }
    }
}


void error_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}


void child_process(int sockfd)
{
    FILE *file;
    char ch;
    char word[MAX_WORD_LENGTH];
    uint8_t length = 0;

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
                send_word(sockfd, word, length);
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
        send_word(sockfd, word, length);
    }

    send_word(sockfd, NULL, 0);

    if(fclose(file) != 0)
    {
        error_exit("Error closing file");
    }

    close(sockfd);
    exit(EXIT_SUCCESS);
}


void parent_process(int sockfd)
{
    uint8_t length;
    char word[MAX_WORD_LENGTH];
    ssize_t read_bytes;

    while(1)
    {
        read_bytes = read(sockfd, &length, sizeof(length));

        if(read_bytes < 0)
        {
            error_exit("Error reading word length from socket");
        }

        if(length == 0)
        {
            break;
        }

        read_bytes = read(sockfd, word, length);

        if(read_bytes < 0)
        {
            error_exit("Error reading word from socket");
        }

        word[length] = '\0';
        printf("Parent: received word of length %u: %s\n", length, word);
    }

    close(sockfd);
    exit(EXIT_SUCCESS);
}
