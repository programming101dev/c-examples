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

static void print_process_info(const char *name);

int main(void)
{
    pid_t pid;

    pid = fork();

    if(pid == -1)
    {
        perror("Error creating child process");
        return EXIT_FAILURE;
    }

    if(pid == 0)
    {
        // This is the child process
        print_process_info("Child");
        printf("Child process finished.\n");
    }
    else
    {
        int status;

        // This is the parent process
        print_process_info("Parent");
        printf("Parent is waiting for the child\n");

        // Wait for the child process to finish
        if(waitpid(pid, &status, 0) == -1)
        {
            perror("Error waiting for child process");
            return EXIT_FAILURE;
        }

        printf("Parent process finished.\n");
    }

    return EXIT_SUCCESS;
}

static void print_process_info(const char *name)
{
    printf("Name: %s, Process: PID=%d, Parent PID=%d\n", name, getpid(), getppid());
}
