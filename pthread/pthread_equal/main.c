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
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);

    // Check if thread1 and thread1 have the same thread ID
    if(pthread_equal(thread1, thread1))
    {
        printf("Thread 1 and Thread 1 have the same thread ID.\n");
    }
    else
    {
        printf("Thread 1 and Thread 1 have different thread IDs.\n");
    }

    // Check if thread1 and thread2 have the same thread ID
    if(pthread_equal(thread1, thread2))
    {
        printf("Thread 1 and Thread 2 have the same thread ID.\n");
    }
    else
    {
        printf("Thread 1 and Thread 2 have different thread IDs.\n");
    }

    // Check if thread1 has the same thread ID as itself
    if(pthread_equal(thread1, pthread_self()))
    {
        printf("Thread 1 and the main thread have the same thread ID.\n");
    }
    else
    {
        printf("Thread 1 and the main thread have different thread IDs.\n");
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Main thread is done.\n");

    return EXIT_SUCCESS;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// cppcheck-suppress constParameterCallback
static void *thread_function(void *arg)
{
    return NULL;
}

#pragma GCC diagnostic pop
