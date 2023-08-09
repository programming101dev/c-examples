#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <unistd.h>


#define SOCKET_PATH "/tmp/example_socket" // Replace with your desired socket path

volatile int running = 1;

static void signal_handler(int signum);

static void signal_handler(int signum)
{
    if(signum == SIGINT) {
        running = 0;
    }
}

int main(void)
{
    int server_socket, *client_sockets = NULL;
    int max_clients = 0;
    int max_fd, activity, i, valread, new_socket, sd;
    int addrlen;
    struct sockaddr_un address;
    fd_set readfds;

    // Create server socket
    if((server_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Set server address parameters
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);

    // Enable address reuse
    int opt = 1;
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("Setsockopt error");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the address
    if(bind(server_socket, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if(listen(server_socket, SOMAXCONN) == -1) {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    printf("Server listening for incoming connections...\n");

    // Set up the signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, signal_handler);

    while(running) {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add the server socket to the set
        FD_SET(server_socket, &readfds);
        max_fd = server_socket;

        // Add the client sockets to the set
        for(i = 0; i < max_clients; i++) {
            sd = client_sockets[i];

            if(sd > 0) {
                FD_SET(sd, &readfds);
            }

            if(sd > max_fd) {
                max_fd = sd;
            }
        }

        // Use select to monitor sockets for read readiness
        activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if(activity < 0) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        // Handle new client connections
        if(FD_ISSET(server_socket, &readfds)) {
            if((new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) == -1) {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }

            printf("New connection established\n");

            // Increase the size of the client_sockets array
            max_clients++;
            client_sockets = realloc(client_sockets, sizeof(int) * max_clients);
            client_sockets[max_clients - 1] = new_socket;
        }

        // Handle incoming data from existing clients
        for(i = 0; i < max_clients; i++) {
            sd = client_sockets[i];

            if(FD_ISSET(sd, &readfds)) {
                char word_length;
                char word[256];

                // Receive the word length (uint8_t)
                valread = read(sd, &word_length, sizeof(word_length));
                if(valread <= 0) {
                    // Connection closed or error
                    printf("Client %d disconnected\n", sd);
                    close(sd);
                    FD_CLR(sd, &readfds); // Remove the closed socket from the set
                    client_sockets[i] = 0;
                } else {
                    // Receive the word based on the length received
                    valread = read(sd, word, (size_t)word_length);
                    if(valread <= 0) {
                        // Connection closed or error
                        printf("Client %d disconnected\n", sd);
                        close(sd);
                        FD_CLR(sd, &readfds); // Remove the closed socket from the set
                        client_sockets[i] = 0;
                    } else {
                        // Null-terminate the word and print it
                        word[valread] = '\0';
                        printf("Received word from client %d: %s\n", sd, word);
                    }
                }
            }
        }
    } // End of the main loop

    // Cleanup and close all client sockets
    for(i = 0; i < max_clients; i++) {
        sd = client_sockets[i];
        if(sd > 0) {
            close(sd);
        }
    }

    // Free the client_sockets array
    free(client_sockets);

    // Close the server socket
    close(server_socket);

    // Remove the socket file
    unlink(SOCKET_PATH);

    printf("Server exited successfully.\n");
    return EXIT_SUCCESS;
}
