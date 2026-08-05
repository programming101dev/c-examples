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

#include <assert.h>
#include <stdlib.h>

int main(void)
{
    int x;
    int y;

    x = 10;    // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    y = 20;    // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

    // If the condition is false, the program will terminate with an error message.
    // If the condition is true, the program continues normally.
    assert(x < y);
    assert(x > y);

    return EXIT_SUCCESS;
}
