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
    int       thread_args[3] = {1, 2, 3};

    for(int i = 0; i < 3; i++)
    {
        pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
    }

    // Wait for all threads to finish and retrieve their exit values
    for(int i = 0; i < 3; i++)
    {
        int *result;

        pthread_join(threads[i], (void **)&result);
        printf("Thread %d returned: %d\n", i + 1, *result);
        free(result);    // Don't forget to free the memory returned by the thread
    }

    printf("Main thread is done.\n");

    return EXIT_SUCCESS;
}

// cppcheck-suppress constParameterCallback
static void *thread_function(void *arg)
{
    int  thread_id;
    int *result;

    thread_id = *(int *)arg;
    printf("Thread %d is executing.\n", thread_id);

    // Perform some work in the thread...

    printf("Thread %d is done.\n", thread_id);

    // Exit the thread and return a value (optional)
    result = (int *)malloc(sizeof(int));

    if(result == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    *result = thread_id * 2;
    pthread_exit(result);
}
