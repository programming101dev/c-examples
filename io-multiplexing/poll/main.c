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

#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/example_socket"
#define MAX_WORD_LEN 256

static void setup_signal_handler(void);
static void sigint_handler(int signum);
static int  socket_create(void);
static void socket_bind(int sockfd, const char *path);
static void socket_close(int sockfd);

static size_t min_size(size_t a, size_t b);

static struct pollfd *initialize_pollfds(int sockfd, int **client_sockets, nfds_t *client_capacity);
static void           handle_new_connection(int sockfd, int **client_sockets, nfds_t *client_count, nfds_t *client_capacity, struct pollfd **fds);
static int            read_full(int fd, void *buf, size_t n);
static void           handle_client_data(struct pollfd *fds, int *client_sockets, nfds_t *client_count);
static void           handle_client_disconnection(int *client_sockets, nfds_t *client_count, struct pollfd *fds, nfds_t client_index);

static volatile sig_atomic_t exit_flag = 0;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

int main(void)
{
    int            sockfd;
    int           *client_sockets  = NULL;
    nfds_t         client_count    = 0;
    nfds_t         client_capacity = 0;
    struct pollfd *fds             = NULL;

    setup_signal_handler();
    unlink(SOCKET_PATH);

    sockfd = socket_create();
    socket_bind(sockfd, SOCKET_PATH);

    if(listen(sockfd, SOMAXCONN) == -1)
    {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    printf("Server listening for incoming connections...\n");

    fds = initialize_pollfds(sockfd, &client_sockets, &client_capacity);

    while(!exit_flag)
    {
        int activity = poll(fds, client_count + 1, -1);

        if(activity < 0)
        {
            perror("Poll error");
            exit(EXIT_FAILURE);
        }

        // Handle new client connections
        handle_new_connection(sockfd, &client_sockets, &client_count, &client_capacity, &fds);

        // Handle incoming data from existing clients
        if(client_count > 0)
        {
            handle_client_data(fds, client_sockets, &client_count);
        }

        // Defensive hygiene: poll() sets these bits; clear them for the next iteration.
        fds[0].revents = 0;
    }

    free(fds);

    // Cleanup and close all active client sockets
    if(client_sockets != NULL)
    {
        for(nfds_t i = 0; i < client_count; i++)
        {
            if(client_sockets[i] > 0)
            {
                socket_close(client_sockets[i]);
            }
        }
        free(client_sockets);
    }

    socket_close(sockfd);
    unlink(SOCKET_PATH);
    printf("Server exited successfully.\n");

    return EXIT_SUCCESS;
}

static size_t min_size(size_t a, size_t b)
{
    return (a < b) ? a : b;
}

static void setup_signal_handler(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    sa.sa_handler = sigint_handler;
#ifdef __clang__
    #pragma clang diagnostic pop
#endif
    (void)sigaction(SIGINT, &sa, NULL);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void sigint_handler(int signum)
{
    exit_flag = 1;
    printf("SIGINT received. Exiting...\n");
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

    printf("Socket created successfully.\n");

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

    printf("Socket closed.\n");
}

static struct pollfd *initialize_pollfds(int sockfd, int **client_sockets, nfds_t *client_capacity)
{
    struct pollfd *fds;

    // Ensure non-zero initial capacity to avoid realloc(..., 0) paths.
    *client_capacity = 4;
    *client_sockets  = (int *)malloc((*client_capacity) * sizeof(int));

    if(*client_sockets == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for(nfds_t i = 0; i < *client_capacity; i++)
    {
        (*client_sockets)[i] = -1;
    }

    fds = (struct pollfd *)malloc((*client_capacity + 1) * sizeof(struct pollfd));

    if(fds == NULL)
    {
        perror("malloc");
        free(*client_sockets);
        exit(EXIT_FAILURE);
    }

    fds[0].fd      = sockfd;
    fds[0].events  = POLLIN;
    fds[0].revents = 0;

    for(nfds_t i = 1; i <= *client_capacity; i++)
    {
        fds[i].fd      = -1;
        fds[i].events  = 0;
        fds[i].revents = 0;
    }

    return fds;
}

static void handle_new_connection(int sockfd, int **client_sockets, nfds_t *client_count, nfds_t *client_capacity, struct pollfd **fds)
{
    if((*fds)[0].revents & POLLIN)
    {
        socklen_t          addrlen;
        int                new_socket;
        struct sockaddr_un addr;

        addrlen    = sizeof(addr);
        new_socket = accept(sockfd, (struct sockaddr *)&addr, &addrlen);

        if(new_socket == -1)
        {
            perror("Accept error");
            exit(EXIT_FAILURE);
        }

        // Grow arrays if needed (guard against 0-capacity for analyzer/portability)
        if(*client_count == *client_capacity)
        {
            nfds_t         new_cap;
            int           *tmp_cs;
            struct pollfd *tmp_fds;

            if(*client_capacity == 0)
            {
                new_cap = 4;
            }
            else
            {
                new_cap = (*client_capacity) * 2;
            }

            tmp_cs = (int *)realloc(*client_sockets, (size_t)new_cap * sizeof(int));
            if(tmp_cs == NULL)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            *client_sockets = tmp_cs;

            tmp_fds = (struct pollfd *)realloc(*fds, ((size_t)new_cap + 1U) * sizeof(struct pollfd));
            if(tmp_fds == NULL)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            *fds = tmp_fds;

            // Initialize the new slots
            for(nfds_t i = *client_capacity; i < new_cap; i++)
            {
                (*client_sockets)[i] = -1;

                (*fds)[i + 1].fd      = -1;
                (*fds)[i + 1].events  = 0;
                (*fds)[i + 1].revents = 0;
            }

            *client_capacity = new_cap;
        }

        // Append the new client at index client_count
        (*client_sockets)[*client_count]    = new_socket;
        (*fds)[(*client_count) + 1].fd      = new_socket;
        (*fds)[(*client_count) + 1].events  = POLLIN;
        (*fds)[(*client_count) + 1].revents = 0;

        (*client_count)++;
    }
}

static int read_full(int fd, void *buf, size_t n)
{
    size_t off = 0;

    while(off < n)
    {
        ssize_t r = read(fd, (char *)buf + off, n - off);

        if(r == 0)
        {
            return 0;
        }

        if(r < 0)
        {
            if(errno == EINTR)
            {
                continue;
            }
            return -1;
        }

        off += (size_t)r;
    }

    return 1;
}

static void handle_client_data(struct pollfd *fds, int *client_sockets, nfds_t *client_count)
{
    // Protocol length field is 1 byte; also leave room for NUL terminator.
    const size_t max_payload = min_size((size_t)(MAX_WORD_LEN - 1U), 255U);

    for(nfds_t i = 0; i < *client_count; i++)
    {
        if(client_sockets[i] != -1 && (fds[i + 1].revents & POLLIN))
        {
            char    word[MAX_WORD_LEN];
            uint8_t word_length_u8;
            int     rc;
            size_t  word_length;

            rc = read_full(client_sockets[i], &word_length_u8, sizeof(word_length_u8));
            if(rc <= 0)
            {
                printf("Client %d disconnected\n", client_sockets[i]);
                handle_client_disconnection(client_sockets, client_count, fds, i);
                continue;
            }

            word_length = (size_t)word_length_u8;

            if(word_length > max_payload)
            {
                printf("Client %d sent oversize word length (%zu)\n", client_sockets[i], word_length);
                handle_client_disconnection(client_sockets, client_count, fds, i);
                continue;
            }

            rc = read_full(client_sockets[i], word, word_length);
            if(rc <= 0)
            {
                printf("Client %d disconnected\n", client_sockets[i]);
                handle_client_disconnection(client_sockets, client_count, fds, i);
                continue;
            }

            word[word_length] = '\0';
            printf("Received word from client %d: %s\n", client_sockets[i], word);

            // Clear after handling to avoid re-processing stale bits
            fds[i + 1].revents = 0;
        }
    }
}

static void handle_client_disconnection(int *client_sockets, nfds_t *client_count, struct pollfd *fds, nfds_t client_index)
{
    int    disconnected_socket = client_sockets[client_index];
    nfds_t last;

    (void)close(disconnected_socket);

    // O(1) removal: move last active client into the removed slot
    last = (*client_count) - 1;

    if(client_index != last)
    {
        client_sockets[client_index] = client_sockets[last];
        fds[client_index + 1]        = fds[last + 1];
    }

    // Clear the last slot
    client_sockets[last]  = -1;
    fds[last + 1].fd      = -1;
    fds[last + 1].events  = 0;
    fds[last + 1].revents = 0;

    (*client_count)--;
}
