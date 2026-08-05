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
#include <unistd.h>

int main(void)
{
    pid_t pid;

    pid = getpid();
    printf("Current Process ID: %d\n", pid);

    // Execute the "ls -l" command using execl
    // The first argument is the path to the command (in this case, "ls")
    // The subsequent arguments are the command-line arguments to "ls" (in this case, "-l" for long format listing)
    if(execl("/bin/ls", "ls", "-l", (char *)NULL) == -1)
    {
        perror("execl");
        exit(EXIT_FAILURE);
    }

    // The code below this line will not be executed, as the process image is replaced by the new program.
    // However, it's a good practice to include it for completeness.

    printf("This line will not be executed if execl is successful.\n");

    return EXIT_SUCCESS;
}
