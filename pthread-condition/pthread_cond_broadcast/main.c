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

struct synchronization_data
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond_var;
    int             shared_data;
};

static void *thread_function(void *arg);

#define NUM_THREADS 3
#define ITERATIONS 10

int main(void)
{
    pthread_t                   threads[NUM_THREADS];
    long                        t;
    struct synchronization_data data;

    pthread_mutex_init(&data.mutex, NULL);
    pthread_cond_init(&data.cond_var, NULL);
    data.shared_data = 0;

    for(t = 0; t < NUM_THREADS; t++)
    {
        int rc;

        rc = pthread_create(&threads[t], NULL, thread_function, (void *)&data);

        if(rc)
        {
            printf("ERROR: Return code from pthread_create() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    // Simulate some work and update the shared_data variable
    for(int i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&data.mutex);
        data.shared_data = i + 1;
        printf("Main thread updating shared_data: %d\n", data.shared_data);
        pthread_cond_broadcast(&data.cond_var);
        pthread_mutex_unlock(&data.mutex);
    }

    // Wait for all threads to finish
    for(t = 0; t < NUM_THREADS; t++)
    {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.cond_var);
    pthread_exit(NULL);
}

static void *thread_function(void *arg)
{
    struct synchronization_data *data;
    pthread_t                    thread_id;

    thread_id = pthread_self();

    data = (struct synchronization_data *)arg;
    pthread_mutex_lock(&data->mutex);

    while(data->shared_data < ITERATIONS)
    {
        printf("Thread %lu is waiting...\n", (unsigned long)thread_id);
        pthread_cond_wait(&data->cond_var, &data->mutex);
        printf("Thread %lu is awake and running. Shared data: %d\n", (unsigned long)thread_id, data->shared_data);
    }

    pthread_mutex_unlock(&data->mutex);
    pthread_exit(NULL);
}
