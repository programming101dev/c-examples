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
#include <sys/socket.h>

int main(void)
{
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    // Hard-coded binary representations of an IPv4 address and an IPv6 address
    unsigned char ipv4_bytes[4]  = {192, 168, 0, 1};
    unsigned char ipv6_bytes[16] = {32, 1, 13, 184, 133, 163, 0, 0, 0, 0, 138, 46, 3, 112, 115, 52};
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    char ipv4_str[INET_ADDRSTRLEN];
    char ipv6_str[INET6_ADDRSTRLEN];

    // Convert IPv4 address from binary format to presentation format (string)
    inet_ntop(AF_INET, ipv4_bytes, ipv4_str, sizeof(ipv4_str));
    printf("IPv4 Address: %s\n", ipv4_str);

    // Convert IPv6 address from binary format to presentation format (string)
    inet_ntop(AF_INET6, ipv6_bytes, ipv6_str, sizeof(ipv6_str));
    printf("IPv6 Address: %s\n", ipv6_str);

    return EXIT_SUCCESS;
}
