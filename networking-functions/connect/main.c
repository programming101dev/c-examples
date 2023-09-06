#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


static int create_socket(void);
static struct sockaddr_in prepare_server_address(int client_fd);
static void connect_to_server(int client_fd, struct sockaddr_in server_addr);
static void do_communication(int client_fd);
static void close_socket(int client_fd);


#define SERVER_IP "127.0.0.1"
#define PORT 8080


int main(void)
{
    int client_fd = create_socket();
    struct sockaddr_in server_addr = prepare_server_address(client_fd);
    connect_to_server(client_fd, server_addr);
    printf("Connected to the server\n");
    do_communication(client_fd);
    close_socket(client_fd);

    return EXIT_SUCCESS;
}


static int create_socket(void)
{
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(client_fd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    return client_fd;
}


static struct sockaddr_in prepare_server_address(int client_fd)
{
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address/Address not supported");
        close_socket(client_fd);
        exit(EXIT_FAILURE);
    }

    return server_addr;
}


static void connect_to_server(int client_fd, struct sockaddr_in server_addr)
{
    if(connect(client_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection failed");
        close_socket(client_fd);
        exit(EXIT_FAILURE);
    }
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void do_communication(int client_fd)
{
    // Perform communication or other actions here
}

#pragma GCC diagnostic pop


static void close_socket(int client_fd)
{
    if(close(client_fd) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}
