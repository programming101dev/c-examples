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


#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


#define PORT 8080
#define BACKLOG 5

static int create_socket(void);

static void bind_socket(int server_fd);

static void start_listening(int server_fd);

static int accept_connection(int server_fd);

int main(void)
{
    int server_fd;
    int client_fd;

    server_fd = create_socket();
    bind_socket(server_fd);
    start_listening(server_fd);
    client_fd = accept_connection(server_fd);

    if(close(client_fd) < 0)
    {
        perror("Failed to close client socket");
        return EXIT_FAILURE;
    }

    if(close(server_fd) < 0)
    {
        perror("Failed to close server socket");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static int create_socket(void)
{
    int server_fd;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd == -1)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

static void bind_socket(int server_fd)
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Socket bound to port %d\n", PORT);
}

static void start_listening(int server_fd)
{
    if(listen(server_fd, BACKLOG) == -1)
    {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Listening for incoming connections...\n");
}

static int accept_connection(int server_fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_len);
    if(client_fd == -1)
    {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Accepted a new connection\n");

    return client_fd;
}
