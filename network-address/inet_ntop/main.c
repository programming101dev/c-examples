/*
 * Copyright 2026 D'Arcy Smith
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
