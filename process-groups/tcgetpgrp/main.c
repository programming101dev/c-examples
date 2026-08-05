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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t foreground_pgid;
    int   terminal_fd;

    terminal_fd = open("/dev/tty", O_RDWR | O_CLOEXEC);    // Open the controlling terminal

    if(terminal_fd == -1)
    {
        perror("Error opening terminal");
        return 1;
    }

    foreground_pgid = tcgetpgrp(terminal_fd);

    if(foreground_pgid == -1)
    {
        perror("Error getting foreground PGID");
        close(terminal_fd);
        return 1;
    }

    printf("Foreground Process Group ID (PGID) of the terminal: %d\n", foreground_pgid);
    close(terminal_fd);

    return EXIT_SUCCESS;
}
