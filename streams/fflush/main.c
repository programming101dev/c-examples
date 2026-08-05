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
    // Fully buffered - probably won't print because the buffer won't get full
    setvbuf(stdout, NULL, _IOFBF, 0);
    printf("This is part one... ");
    sleep(2);

    // flush the stream to force the output
    fflush(stdout);
    printf("and this is part two.\n");
    sleep(2);

    // no fflush needed - it happens when the program exits.

    return EXIT_SUCCESS;
}
