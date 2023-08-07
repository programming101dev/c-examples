#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/example_socket"

int main(void) {
    int sockfd;
    struct sockaddr_un server_addr;

    // Create a socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    // Connect to the server
    if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Open the file to read
    FILE *file = fopen("../example.txt", "r");
    if(file == NULL) {
        perror("fopen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Read and parse words from the file
    char line[1024]; // Adjust the buffer size as needed
    while(fgets(line, sizeof(line), file) != NULL) {
        char *word;
        word = strtok(line, " \t\n");
        while(word != NULL) {
            size_t word_len = strlen(word);
            if(word_len > UINT8_MAX) {
                fprintf(stderr, "Word exceeds maximum length\n");
                fclose(file);
                close(sockfd);
                exit(EXIT_FAILURE);
            }

            // Write the size of the word as uint8_t
            uint8_t size = (uint8_t)word_len;
            send(sockfd, &size, sizeof(uint8_t), 0);

            // Write the word
            write(sockfd, word, word_len);

            word = strtok(NULL, " \t\n");
        }
    }

    fclose(file);
    close(sockfd);
    return EXIT_SUCCESS;
}
