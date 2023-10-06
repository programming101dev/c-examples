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

int main(int argc, char *argv[])
{
    uint16_t network_value;
    uint16_t host_value;

    network_value = 0x3039;    // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    host_value    = ntohs(network_value);
    printf("Network value: %hu\n", network_value);
    printf("Host value: %hu\n", host_value);

    return EXIT_SUCCESS;
}
