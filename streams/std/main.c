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

int main(void)
{
    int stdin_fd;
    int stdout_fd;
    int stderr_fd;

    stdin_fd  = fileno(stdin);
    stdout_fd = fileno(stdout);
    stderr_fd = fileno(stderr);
    printf("File descriptor for stdin: %d\n", stdin_fd);
    printf("File descriptor for stdout: %d\n", stdout_fd);
    printf("File descriptor for stderr: %d\n", stderr_fd);

    return EXIT_SUCCESS;
}
