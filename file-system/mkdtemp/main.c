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
    // TODO pass on the command line
    char tempdir_template[] = "exampleXXXXXX";

    if(mkdtemp(tempdir_template) == NULL)
    {
        perror("Error creating temporary directory");
        return EXIT_FAILURE;
    }

    printf("Temporary directory created: %s\n", tempdir_template);

    if(rmdir(tempdir_template) == -1)
    {
        perror("Error removing temporary directory");
        return EXIT_FAILURE;
    }

    printf("Temporary directory removed: %s\n", tempdir_template);

    return EXIT_SUCCESS;
}
