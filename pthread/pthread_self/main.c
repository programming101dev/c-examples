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

static void *thread_function(void *arg);

int main(void)
{
    pthread_t threads[3];

    for(int i = 0; i < 3; i++)
    {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    for(int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Main thread is done.\n");

    return EXIT_SUCCESS;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// cppcheck-suppress constParameterCallback
static void *thread_function(void *arg)
{
    pthread_t thread_id;

    thread_id = pthread_self();
    printf("Thread ID: %lu\n", (unsigned long)thread_id);

    // Perform some work in the thread...

    return NULL;
}

#pragma GCC diagnostic pop
