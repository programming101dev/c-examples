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

static void exit_handler1(void);
static void exit_handler2(void);
static void exit_handler3(void);

int main(void)
{
    atexit(exit_handler1);
    atexit(exit_handler2);
    atexit(exit_handler3);
    printf("Main function executing.\n");

    return EXIT_SUCCESS;
}

static void exit_handler1(void)
{
    printf("exit_handler1 called.\n");
}

static void exit_handler2(void)
{
    printf("exit_handler2 called.\n");
}

static void exit_handler3(void)
{
    printf("exit_handler3 called.\n");
}
