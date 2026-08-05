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

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define MESSAGE_LEN 256

int main(void)
{
    struct termios options;
    char           message[MESSAGE_LEN];

    tcgetattr(STDOUT_FILENO, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(STDOUT_FILENO, TCSANOW, &options);
    printf("Enter your message: ");
    fflush(stdout);

    // Read the message from stdin
    fgets(message, sizeof(message), stdin);

    // Write the message to stdout (which will be the terminal)
    printf("You wrote: %s\n", message);
    fflush(stdout);

    // Wait until all data is transmitted
    if(tcdrain(STDOUT_FILENO) != 0)
    {
        perror("Error waiting for data to be transmitted");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
