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
#include <sys/wait.h>
#include <unistd.h>

static void handle_child(void);

int main(void)
{
    pid_t pid;
    pid_t pgid;
    pid_t child_pid;

    pid  = getpid();    // Get the PID of the current process
    pgid = getpgid(pid);
    printf("Original Process ID (PID): %d\n", pid);
    printf("Original Process Group ID (PGID): %d\n", pgid);
    child_pid = fork();

    if(child_pid < 0)
    {
        perror("Error forking a new process");
        return EXIT_FAILURE;
    }

    if(child_pid == 0)
    {
        handle_child();
    }
    else
    {
        // Wait for the child process to finish
        wait(NULL);
        printf("Parent Process ID (PID) after child process execution: %d\n", getpid());
    }

    return EXIT_SUCCESS;
}

static void handle_child(void)
{
    pid_t child_pid;
    pid_t child_pgid;
    pid_t new_pgid;
    pid_t sid;

    child_pid  = getpid();
    child_pgid = getpgid(child_pid);
    printf("Child Process ID (PID): %d\n", child_pid);
    printf("Child Process Group ID (PGID): %d\n", child_pgid);

    // Create a new session for the child process
    sid = setsid();

    if(sid == -1)
    {
        perror("Error creating a new session for the child process");
        exit(EXIT_FAILURE);
    }

    printf("Child Process New Session ID (SID): %d\n", sid);
    new_pgid = getpgid(child_pid);
    printf("Child Process New Process Group ID (PGID): %d\n", new_pgid);
}
