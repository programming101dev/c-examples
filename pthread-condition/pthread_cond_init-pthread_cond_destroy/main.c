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

int main(void)
{
    pthread_cond_t condition;
    int            destroy_result;
    int            init_result;

    init_result = pthread_cond_init(&condition, NULL);

    if(init_result != 0)
    {
        fprintf(stderr, "Error initializing condition variable. Error code: %d\n", init_result);
        return EXIT_FAILURE;
    }

    printf("condition created\n");

    // Destroy the condition variable
    destroy_result = pthread_cond_destroy(&condition);

    if(destroy_result != 0)
    {
        fprintf(stderr, "Error destroying condition variable. Error code: %d\n", destroy_result);
        return EXIT_FAILURE;
    }

    printf("condition destroyed\n");

    return EXIT_SUCCESS;
}
