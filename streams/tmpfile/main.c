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
    FILE *tempFile;

    tempFile = tmpfile();

    if(tempFile == NULL)
    {
        perror("Error creating temporary file");
        return EXIT_FAILURE;
    }

    printf("Temp file created\n");

    // The temporary file will be automatically deleted when the program terminates
    // or when you explicitly close the file using fclose:
    if(fclose(tempFile) == EOF)
    {
        perror("Error closing temporary file");
        return EXIT_FAILURE;
    }

    printf("Temp file closed\n");

    return EXIT_SUCCESS;
}
