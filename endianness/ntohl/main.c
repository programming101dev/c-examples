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

int main(void)
{
    uint32_t network_value;
    uint32_t host_value;

    network_value = 0x78563412;    // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    host_value    = ntohl(network_value);
    printf("Network value: %u\n", network_value);
    printf("Host value: %u\n", host_value);

    return EXIT_SUCCESS;
}
