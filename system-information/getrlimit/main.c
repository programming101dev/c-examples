/*
 * This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International license.
 *
 * Author: D'Arcy Smith (ds@programming101.dev)
 *
 * You are free to:
 *   - Share: Copy and redistribute the material in any medium or format.
 *   - Under the following terms:
 *       - Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
 *       - NonCommercial: You may not use the material for commercial purposes.
 *       - NoDerivatives: If you remix, transform, or build upon the material, you may not distribute the modified material.
 *
 * For more details, please refer to the full license text at:
 * https://creativecommons.org/licenses/by-nc-nd/4.0/
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

static void show_limit(int resource, const char *name);
#define SHOW_LIMIT(resource) show_limit(resource, #resource)

int main(int argc, char *argv[])
{
#if defined(__clang__)
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
#if defined(__clang__)
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
#elif defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wsign-conversion"
    #pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    if(getrlimit(resource, &rlim) == 0)
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic pop
#elif defined(__clang__)
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
