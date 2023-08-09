#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <poll.h>
#include <errno.h>

#define SOCKET_PATH "/tmp/example_socket" // Replace with your desired socket path

volatile int running = 1;

void signal_handler(int signum);
int create_server_socket(void);
void handle_new_client(int server_socket, int **client_sockets, int *max_clients);
void handle_client_data(int sd, int **client_sockets, int *max_clients);
void setup_signal_handler(void);


void signal_handler(int signum)
{
    if(signum == SIGINT)
    {
        printf("got sigint\n");
        running = 0;
    }
}

int create_server_socket(void)
{
    int server_socket;

    // Create server socket
    if((server_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Set server address parameters
    struct sockaddr_un address;
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);

    // Enable address reuse
    int opt = 1;
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        perror("Setsockopt error");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the address
    if(bind(server_socket, (struct sockaddr *) &address, sizeof(address)) == -1)
    {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if(listen(server_socket, SOMAXCONN) == -1)
    {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    printf("Server listening for incoming connections...\n");

    return server_socket;
}

void handle_new_client(int server_socket, int **client_sockets, int *max_clients)
{
    // Initialize the client address length
    struct sockaddr_un address;
    socklen_t client_len = sizeof(address);

    int new_socket = accept(server_socket, (struct sockaddr *) &address, &client_len);
    if(new_socket == -1)
    {
        perror("Accept error");
        exit(EXIT_FAILURE);
    }

    printf("New connection established\n");

    // Increase the size of the client_sockets array
    (*max_clients)++;
    *client_sockets = realloc(*client_sockets, sizeof(int) * (*max_clients));
    (*client_sockets)[(*max_clients) - 1] = new_socket;
}

void handle_client_data(int sd, int **client_sockets, int *max_clients)
{
    char word_length;
    char word[256];

    // Receive the word length (uint8_t)
    int valread = read(sd, &word_length, sizeof(word_length));
    if(valread <= 0)
    {
        // Connection closed or error
        printf("Client %d disconnected\n", sd);
        close(sd);

        // Mark the disconnected client socket as 0
        int i;
        for(i = 0; i < *max_clients; i++)
        {
            if((*client_sockets)[i] == sd)
            {
                (*client_sockets)[i] = 0;
                break;
            }
        }
    }
    else
    {
        // Receive the word based on the length received
        valread = read(sd, word, (size_t) word_length);
        if(valread <= 0)
        {
            // Connection closed or error
            printf("Client %d disconnected\n", sd);
            close(sd);

            // Mark the disconnected client socket as 0
            int i;
            for(i = 0; i < *max_clients; i++)
            {
                if((*client_sockets)[i] == sd)
                {
                    (*client_sockets)[i] = 0;
                    break;
                }
            }
        }
        else
        {
            // Null-terminate the word and print it
            word[valread] = '\0';
            printf("Received word from client %d: %s\n", sd, word);
        }
    }
}

void setup_signal_handler(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigaction(SIGINT, &sa, NULL);
}

int main(void)
{
    int server_socket, *client_sockets = NULL;
    int max_clients = 0;
    int i, num_ready;
    struct pollfd *fds;

    setup_signal_handler();
    server_socket = create_server_socket();

    // Allocate memory for the fds array
    fds = (struct pollfd *) malloc(sizeof(struct pollfd));

    while(running)
    {
        // Set up the pollfd structure for the server socket
        fds[0].fd = server_socket;
        fds[0].events = POLLIN;

        // Set up the pollfd structures for all client sockets
        for(i = 0; i < max_clients; i++)
        {
            int sd = client_sockets[i];
            fds[i + 1].fd = sd;
            fds[i + 1].events = POLLIN;
        }

        // Use poll to monitor all sockets for activity
        num_ready = poll(fds, max_clients + 1, -1);

        if(num_ready < 0)
        {
            if(errno == EINTR)
            {
                // The poll call was interrupted by a signal (e.g., SIGINT)
                // Continue the loop and retry the poll call
                continue;
            }
            else
            {
                perror("Poll error");
                exit(EXIT_FAILURE);
            }
        }

        // Handle new client connections
        if(fds[0].revents & POLLIN)
        {
            handle_new_client(server_socket, &client_sockets, &max_clients);

            // Increase the size of the fds array
            fds = (struct pollfd *) realloc(fds, sizeof(struct pollfd) * (max_clients + 1));
        }

        // Handle incoming data from existing clients
        for(i = 0; i < max_clients; i++)
        {
            int sd = client_sockets[i];

            if(fds[i + 1].revents & POLLIN)
            {
                handle_client_data(sd, &client_sockets, &max_clients);
            }
        }
    }

    printf("Cleaning up\n");

    // Cleanup and close all client sockets
    for(i = 0; i < max_clients; i++)
    {
        int sd = client_sockets[i];

        if(sd > 0)
        {
            close(sd);
        }
    }

    // Free the client_sockets array
    free(client_sockets);

    // Free the fds array
    free(fds);

    // Close the server socket
    close(server_socket);

    // Remove the socket file
    unlink(SOCKET_PATH);

    printf("Server exited successfully.\n");
    return EXIT_SUCCESS;
}
