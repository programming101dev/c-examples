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
#include <sys/wait.h>
#include <unistd.h>

// TODO read this in off the command line
#define XXX 100

int main(void)
{
    pid_t new_pgid;
    pid_t child_pid;
    int   terminal_fd;

    terminal_fd = open("/dev/tty", O_RDWR | O_CLOEXEC);    // Open the controlling terminal

    if(terminal_fd == -1)
    {
        perror("Error opening terminal");
        return 1;
    }

    new_pgid = getpid();

    printf("Original Process ID (PID): %d\n", new_pgid);
    child_pid = fork();

    if(child_pid < 0)
    {
        perror("Error forking a new process");
        close(terminal_fd);
        return EXIT_FAILURE;
    }

    if(child_pid == 0)
    {
        new_pgid = getpid() + XXX;
        printf("Child Process ID (PID): %d\n", getpid());

        if(setsid() == -1)
        {
            perror("Error creating a new session for the child process");
            close(terminal_fd);
            return EXIT_FAILURE;
        }

        printf("Child Process Group ID (PGID) after creating a new session: %d\n", getpid());

        if(setpgid(getpid(), new_pgid) == -1)
        {
            perror("Error setting PGID for the child process");
            close(terminal_fd);
            return EXIT_FAILURE;
        }

        printf("Child Process Group ID (PGID) after change: %d\n", new_pgid);
        close(terminal_fd);
    }
    else
    {
        // Wait for the child process to finish
        wait(NULL);
        printf("Parent Process ID (PID) after child process execution: %d\n", getpid());
        close(terminal_fd);
    }

    return EXIT_SUCCESS;
}
