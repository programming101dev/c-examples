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
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/example_socket"
#define MAX_WORD_LEN 256
#define PROTOCOL_MAX_PAYLOAD_LEN 255U

static void setup_signal_handler(void);
static void sigint_handler(int signum);
static int  socket_create(void);
static int  socket_bind(int sockfd, const char *path);
static void socket_close(int sockfd);

static size_t min_size(size_t a, size_t b);
static int    read_full(int fd, void *buf, size_t n);

static void grow_clients(int **client_sockets, size_t *client_capacity);
static void remove_client(int *client_sockets, size_t *client_count, size_t idx);

/* Warning-clean wrappers around FD_* macros for -Wsign-conversion -Werror. */
static int  fd_in_select_range(int fd);
static void fd_set_safe(int fd, fd_set *set);
static int  fd_isset_safe(int fd, const fd_set *set);

static volatile sig_atomic_t exit_flag = 0;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

int main(void)
{
    int    sockfd;
    int   *client_sockets  = NULL;
    size_t client_count    = 0;
    size_t client_capacity = 0;

    setup_signal_handler();
    unlink(SOCKET_PATH);

    sockfd = socket_create();

    if(sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if(socket_bind(sockfd, SOCKET_PATH) == -1)
    {
        perror("bind error");
        socket_close(sockfd);
        unlink(SOCKET_PATH);
        exit(EXIT_FAILURE);
    }

    if(listen(sockfd, SOMAXCONN) == -1)
    {
        perror("Listen error");
        socket_close(sockfd);
        unlink(SOCKET_PATH);
        exit(EXIT_FAILURE);
    }

    // select() limitation: all fds must be < FD_SETSIZE
    if(sockfd < 0 || sockfd >= FD_SETSIZE)
    {
        fprintf(stderr, "Server socket fd %d is out of range for select() (FD_SETSIZE=%d)\n", sockfd, FD_SETSIZE);

        if(sockfd >= 0)
        {
            socket_close(sockfd);
        }

        unlink(SOCKET_PATH);
        exit(EXIT_FAILURE);
    }

    printf("Server listening for incoming connections...\n");

    while(!exit_flag)
    {
        fd_set readfds;
        int    max_fd = sockfd;

        FD_ZERO(&readfds);

        fd_set_safe(sockfd, &readfds);

        for(size_t i = 0; i < client_count; i++)
        {
            int sd = client_sockets[i];

            if(sd < 0)
            {
                continue;
            }

            if(sd >= FD_SETSIZE)
            {
                fprintf(stderr, "Client fd %d is out of range for select(); disconnecting\n", sd);
                (void)close(sd);
                remove_client(client_sockets, &client_count, i);
                i--;    // re-check swapped-in element
                continue;
            }

            fd_set_safe(sd, &readfds);
            if(sd > max_fd)
            {
                max_fd = sd;
            }
        }

        {
            int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

            if(activity < 0)
            {
                if(errno == EINTR)
                {
                    continue;
                }
                perror("Select error");
                exit(EXIT_FAILURE);
            }
        }

        // New incoming connection
        if(fd_isset_safe(sockfd, &readfds))
        {
            struct sockaddr_un addr;
            socklen_t          addrlen = sizeof(addr);
            int                new_socket;

            new_socket = accept(sockfd, (struct sockaddr *)&addr, &addrlen);

            if(new_socket == -1)
            {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }

            if(new_socket >= FD_SETSIZE)
            {
                fprintf(stderr, "Accepted client fd %d out of range for select(); closing\n", new_socket);
                (void)close(new_socket);
            }
            else
            {
                if(client_count == client_capacity)
                {
                    grow_clients(&client_sockets, &client_capacity);
                }

                client_sockets[client_count] = new_socket;
                client_count++;

                printf("New connection established (fd=%d)\n", new_socket);
            }
        }

        // Client IO
        for(size_t i = 0; i < client_count; i++)
        {
            int sd = client_sockets[i];

            if(!fd_in_select_range(sd))
            {
                continue;
            }

            if(fd_isset_safe(sd, &readfds))
            {
                uint8_t word_length_u8;
                char    word[MAX_WORD_LEN];
                size_t  word_length;
                size_t  max_payload;
                int     rc;

                rc = read_full(sd, &word_length_u8, sizeof(word_length_u8));
                if(rc <= 0)
                {
                    printf("Client %d disconnected\n", sd);
                    (void)close(sd);
                    remove_client(client_sockets, &client_count, i);
                    i--;
                    continue;
                }

                word_length = (size_t)word_length_u8;
                max_payload = min_size((size_t)(MAX_WORD_LEN - 1U), PROTOCOL_MAX_PAYLOAD_LEN);

                if(word_length > max_payload)
                {
                    printf("Client %d sent oversize word length (%zu)\n", sd, word_length);
                    (void)close(sd);
                    remove_client(client_sockets, &client_count, i);
                    i--;
                    continue;
                }

                rc = read_full(sd, word, word_length);
                if(rc <= 0)
                {
                    printf("Client %d disconnected\n", sd);
                    (void)close(sd);
                    remove_client(client_sockets, &client_count, i);
                    i--;
                    continue;
                }

                word[word_length] = '\0';
                printf("Received word from client %d: %s\n", sd, word);
            }
        }
    }

    // Cleanup
    for(size_t i = 0; i < client_count; i++)
    {
        if(client_sockets[i] >= 0)
        {
            socket_close(client_sockets[i]);
        }
    }

    free(client_sockets);
    socket_close(sockfd);
    unlink(SOCKET_PATH);
    printf("Server exited successfully.\n");

    return EXIT_SUCCESS;
}

static size_t min_size(size_t a, size_t b)
{
    return (a < b) ? a : b;
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

static void grow_clients(int **client_sockets, size_t *client_capacity)
{
    size_t new_cap;

    if(*client_capacity == 0U)
    {
        new_cap = 4U;
    }
    else
    {
        new_cap = (*client_capacity) * 2U;
    }

    {
        int *tmp = (int *)realloc(*client_sockets, new_cap * sizeof(int));
        if(tmp == NULL)
        {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        *client_sockets = tmp;
    }

    *client_capacity = new_cap;
}

static void remove_client(int *client_sockets, size_t *client_count, size_t idx)
{
    size_t last = (*client_count) - 1U;

    if(idx != last)
    {
        client_sockets[idx] = client_sockets[last];
    }

    (*client_count)--;
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
    }

    return sockfd;
}

static int socket_bind(int sockfd, const char *path)
{
    struct sockaddr_un addr;
    int                ret_val;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
    addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';

    ret_val = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));

    if(ret_val != -1)
    {
        printf("Bound to domain socket: %s\n", path);
    }

    return ret_val;
}

static void socket_close(int sockfd)
{
    if(close(sockfd) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}

static int fd_in_select_range(int fd)
{
    return (fd >= 0) && (fd < FD_SETSIZE);
}

static void fd_set_safe(int fd, fd_set *set)
{
    if(!fd_in_select_range(fd))
    {
        return;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
    FD_SET(fd, set);
#pragma GCC diagnostic pop
}

static int fd_isset_safe(int fd, const fd_set *set)
{
    int rc;

    if(!fd_in_select_range(fd))
    {
        return 0;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
    rc = FD_ISSET(fd, set);
#pragma GCC diagnostic pop

    return rc;
}
