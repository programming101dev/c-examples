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
#include <sys/resource.h>

static void set_rlimit(int resource, const char *name, rlim_t soft_limit, rlim_t hard_limit);
#define SET_LIMIT(resource, soft_limit, hard_limit) set_rlimit(resource, #resource, soft_limit, hard_limit)

int main(void)
{
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    SET_LIMIT(RLIMIT_CORE, 8, 32);
    SET_LIMIT(RLIMIT_CPU, 5, 10);
    // RLIMIT_DATA is informational only and not settable.
    printf("Resource: RLIMIT_DATA\n");
    printf("  Not settable by setrlimit\n");
    SET_LIMIT(RLIMIT_FSIZE, 1048576, 1048576);
    SET_LIMIT(RLIMIT_NOFILE, 1024, 2048);
    SET_LIMIT(RLIMIT_STACK, 8388608, 16777216);
    // TODO - this is problematic
    SET_LIMIT(RLIMIT_AS, 6710886, 6710886);
#ifdef __clang__
    #pragma clang diagnostic pop
#endif
    return EXIT_SUCCESS;
}

static void set_rlimit(int resource, const char *name, rlim_t soft_limit, rlim_t hard_limit)
{
    struct rlimit rlim;

    rlim.rlim_cur = soft_limit;
    rlim.rlim_max = hard_limit;
    printf("Resource: %s\n", name);
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wc++-compat"
#endif
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wsign-conversion"
#endif
    if(setrlimit(resource, &rlim) == -1)
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic pop
#endif
#ifdef __clang__
    #pragma clang diagnostic pop
#endif
    {
        perror("setrlimit");
    }
    else
    {
        printf("  Soft limit set to: %lld\n", (long long)rlim.rlim_cur);
        printf("  Hard limit set to: %lld\n", (long long)rlim.rlim_max);
    }
}
