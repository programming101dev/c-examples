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

int main(void)
{
    int   terminal_fd;
    pid_t sid;

    terminal_fd = STDIN_FILENO;    // Use standard input (stdin) as the terminal
    sid         = tcgetsid(terminal_fd);

    if(sid == -1)
    {
        perror("Error getting SID");
        return 1;
    }

    printf("Session ID (SID) of the foreground process group: %d\n", sid);

    return EXIT_SUCCESS;
}
