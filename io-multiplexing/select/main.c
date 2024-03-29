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

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

static void setup_signal_handler(void);
static void sigint_handler(int signum);
static int  socket_create(void);
static void socket_bind(int sockfd, const char *path);
static void socket_close(int sockfd);

#define SOCKET_PATH "/tmp/example_socket"
#define MAX_WORD_LEN 256

static volatile sig_atomic_t exit_flag = 0;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

int main(void)
{
    int    sockfd;
    int   *client_sockets;
    size_t max_clients;
    int    new_socket;
    int    sd;
    fd_set readfds;

    client_sockets = NULL;
    max_clients    = 0;
    setup_signal_handler();
    unlink(SOCKET_PATH);    // Remove the existing socket file if it exists
    sockfd = socket_create();
    socket_bind(sockfd, SOCKET_PATH);

    if(listen(sockfd, SOMAXCONN) == -1)
    {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    printf("Server listening for incoming connections...\n");

    while(!exit_flag)
    {
        int max_fd;
        int activity;

        // Clear the socket set
#ifndef __clang_analyzer__
        FD_ZERO(&readfds);
#endif

#if defined(__FreeBSD__) && defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
        // Add the server socket to the set
        FD_SET(sockfd, &readfds);
#if defined(__FreeBSD__) && defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif

        max_fd = sockfd;

        // Add the client sockets to the set
        for(size_t i = 0; i < max_clients; i++)
        {
            sd = client_sockets[i];

            if(sd > 0)
            {
#if defined(__FreeBSD__) && defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
                FD_SET(sd, &readfds);
#if defined(__FreeBSD__) && defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
            }
            if(sd > max_fd)
            {
                max_fd = sd;
            }
        }

        activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if(activity < 0)
        {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

#if defined(__FreeBSD__) && defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
        // Handle new client connections
        if(FD_ISSET(sockfd, &readfds))
#if defined(__FreeBSD__) && defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
        {
            int               *temp;
            struct sockaddr_un addr;
            socklen_t          addrlen;

            addrlen = sizeof(addr);

            new_socket = accept(sockfd, (struct sockaddr *)&addr, &addrlen);

            if(new_socket == -1)
            {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }

            printf("New connection established\n");

            // Increase the size of the client_sockets array
            max_clients++;
            temp = (int *)realloc(client_sockets, sizeof(int) * max_clients);

            if(temp == NULL)
            {
                perror("realloc");
                free(client_sockets);
                exit(EXIT_FAILURE);
            }
            else
            {
                client_sockets                  = temp;
                client_sockets[max_clients - 1] = new_socket;
            }
        }

        // Handle incoming data from existing clients
        for(size_t i = 0; i < max_clients; i++)
        {
            sd = client_sockets[i];

#if defined(__FreeBSD__) && defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
            if(FD_ISSET(sd, &readfds))
            {
#if defined(__FreeBSD__) && defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
                uint8_t word_length;
                ssize_t valread;

                // Receive the word length (uint8_t)
                valread = read(sd, &word_length, sizeof(word_length));

                if(valread <= 0)
                {
                    // Connection closed or error
                    printf("Client %d disconnected\n", sd);
                    close(sd);
#if defined(__FreeBSD__) && defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
                    FD_CLR(sd, &readfds);    // Remove the closed socket from the set
#if defined(__FreeBSD__) && defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
                    client_sockets[i] = 0;
                }
                else
                {
                    char word[MAX_WORD_LEN];

                    // Receive the word based on the length received
                    valread = read(sd, word, (size_t)word_length);

                    if(valread <= 0)
                    {
                        // Connection closed or error
                        printf("Client %d disconnected\n", sd);
                        close(sd);
#if defined(__FreeBSD__) && defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
                        FD_CLR(sd, &readfds);    // Remove the closed socket from the set
#if defined(__FreeBSD__) && defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
                        client_sockets[i] = 0;
                    }
                    else
                    {
                        // Null-terminate the word and print it
                        word[valread] = '\0';
                        printf("Received word from client %d: %s\n", sd, word);
                    }
                }
            }
        }
    }

    // Cleanup and close all client sockets
    for(size_t i = 0; i < max_clients; i++)
    {
        sd = client_sockets[i];

        if(sd > 0)
        {
            socket_close(sd);
        }
    }

    free(client_sockets);
    socket_close(sockfd);
    unlink(SOCKET_PATH);
    printf("Server exited successfully.\n");

    return EXIT_SUCCESS;
}

static void setup_signal_handler(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    sa.sa_handler = sigint_handler;
#if defined(__clang__)
    #pragma clang diagnostic pop
#endif
    sigaction(SIGINT, &sa, NULL);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void sigint_handler(int signum)
{
    exit_flag = 1;
}

#pragma GCC diagnostic pop

static int socket_create(void)
{
    int sockfd;

#ifdef SOCK_CLOEXEC
    sockfd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
#else
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);    // NOLINT(android-cloexec-socket)
#endif

    if(sockfd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

static void socket_bind(int sockfd, const char *path)
{
    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
    addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';

    if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("Bound to domain socket: %s\n", path);
}

static void socket_close(int sockfd)
{
    if(close(sockfd) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}
