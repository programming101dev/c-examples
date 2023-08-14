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
    int result = getnameinfo((struct sockaddr *) &addr, addrlen, hostname, NI_MAXHOST, port, NI_MAXSERV,
                             NI_NUMERICSERV);

    if(result == 0)
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
