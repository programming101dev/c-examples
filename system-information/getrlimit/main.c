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

static void show_limit(int resource, const char *name);
#define SHOW_LIMIT(resource) show_limit(resource, #resource)

int main(void)
{
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    SHOW_LIMIT(RLIMIT_CORE);
    SHOW_LIMIT(RLIMIT_CPU);
    SHOW_LIMIT(RLIMIT_DATA);
    SHOW_LIMIT(RLIMIT_FSIZE);
    SHOW_LIMIT(RLIMIT_NOFILE);
    SHOW_LIMIT(RLIMIT_STACK);
    SHOW_LIMIT(RLIMIT_AS);
#ifdef __clang__
    #pragma clang diagnostic pop
#endif
    return EXIT_SUCCESS;
}

static void show_limit(int resource, const char *name)
{
    struct rlimit rlim;
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wc++-compat"
#endif
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wsign-conversion"
    #pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    if(getrlimit(resource, &rlim) == 0)
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic pop
#endif
#ifdef __clang__
    #pragma clang diagnostic pop
#endif
    {
        printf("%s:\n", name);
        printf("\tCurrent soft limit: ");

        if(rlim.rlim_cur == RLIM_INFINITY)
        {
            printf("unlimited\n");
        }
        else
        {
            printf("%lld bytes\n", (long long)rlim.rlim_cur);
        }

        printf("\tCurrent hard limit: ");

        if(rlim.rlim_max == RLIM_INFINITY)
        {
            printf("unlimited\n");
        }
        else
        {
            printf("%lld bytes\n", (long long)rlim.rlim_max);
        }
    }
    else
    {
        perror("Error getting resource limit");
    }
}
