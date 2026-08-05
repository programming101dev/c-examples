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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void *print_numbers(void *arg);

// TODO read this in on the command line
#define NUMBERS_LEN 5

int main(void)
{
    pthread_t thread_id;
    int       result;

    result = pthread_create(&thread_id, NULL, print_numbers, NULL);

    if(result != 0)
    {
        perror("Thread creation failed");
        return 1;
    }

    printf("Main is working\n");
    sleep(2);
    printf("Main is still working\n");
    sleep(2);
    printf("Main is done working, waiting for the thread to finish\n");

    // Wait for the thread to finish execution
    pthread_join(thread_id, NULL);
    printf("Thread execution completed.\n");

    return EXIT_SUCCESS;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// cppcheck-suppress constParameterCallback
static void *print_numbers(void *arg)
{
    // TODO pass the NUMBERS_LEN in the arg variable
    for(int i = 1; i <= NUMBERS_LEN; ++i)
    {
        printf("Thread: %d\n", i);
        sleep(1);
    }

    // The thread returns NULL upon completion
    return NULL;
}

#pragma GCC diagnostic pop
