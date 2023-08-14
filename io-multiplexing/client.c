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
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>


#define SOCKET_PATH "/tmp/example_socket"


static void send_word(int sockfd, const char *word, uint8_t length);
static void error_exit(const char *msg);


int main(void)
{
    int sockfd;
    struct sockaddr_un server_addr;

    // Create a socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    // Connect to the server
    if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Open the file to read
    FILE *file = fopen("../example.txt", "r");
    if(file == NULL)
    {
        perror("fopen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Read and parse words from the file
    char line[1024]; // Adjust the buffer size as needed
    while(fgets(line, sizeof(line), file) != NULL)
    {
        char *word;
        word = strtok(line, " \t\n");
        while(word != NULL)
        {
            size_t word_len = strlen(word);
            if(word_len > UINT8_MAX)
            {
                fprintf(stderr, "Word exceeds maximum length\n");
                fclose(file);
                close(sockfd);
                exit(EXIT_FAILURE);
            }

            // Write the size of the word as uint8_t
            uint8_t size = (uint8_t) word_len;
            send_word(sockfd, word, size);

            word = strtok(NULL, " \t\n");
        }
    }

    fclose(file);
    close(sockfd);
    return EXIT_SUCCESS;
}


static void send_word(int sockfd, const char *word, uint8_t length)
{
    ssize_t written_bytes;

    printf("Client: sending word of length %u: %s\n", length, word);
    written_bytes = send(sockfd, &length, sizeof(uint8_t), 0);

    if(written_bytes < 0)
    {
        error_exit("Error writing word length to socket");
    }

    if(length > 0)
    {
        written_bytes = send(sockfd, word, length, 0);

        if(written_bytes < 0)
        {
            error_exit("Error writing word to socket");
        }
    }

    // Add random delay between 500ms and 1500ms
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 500000000 + (rand() % 1000000000); // 500ms + random nanoseconds
    nanosleep(&delay, NULL);
}


static void error_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
