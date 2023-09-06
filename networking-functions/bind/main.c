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


static int create_socket(void);
static void bind_socket(int server_fd);


#define PORT 8080


int main(void)
{
    int server_fd;

    server_fd = create_socket();
    bind_socket(server_fd);

    if(close(server_fd) < 0)
    {
        perror("Failed to close socket");
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
