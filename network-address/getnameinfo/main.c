#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>


int main(void)
{
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    char hostname[NI_MAXHOST], port[NI_MAXSERV];

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Localhost (127.0.0.1)
    addr.sin_port = htons(8080); // Port number 8080

    // Get the hostname and port number associated with the address
    int result = getnameinfo((struct sockaddr *)&addr, addrlen, hostname, NI_MAXHOST, port, NI_MAXSERV, NI_NUMERICSERV);

    if (result == 0)
    {
        printf("Address: %s:%s\n", hostname, port);
    }
    else
    {
        fprintf(stderr, "getnameinfo error: %s\n", gai_strerror(result));
        return 1;
    }

    return 0;
}
