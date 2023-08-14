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


#define SET_LIMIT(resource, soft_limit, hard_limit) \
    set_rlimit(resource, #resource, soft_limit, hard_limit)


static void set_rlimit(int resource, const char *name, rlim_t soft_limit, rlim_t hard_limit);


int main(void)
{
    SET_LIMIT(RLIMIT_CORE, 8, 32);
    SET_LIMIT(RLIMIT_CPU, 5, 10);
    // RLIMIT_DATA is informational only and not settable.
    printf("Resource: RLIMIT_DATA\n");
    printf("  Not settable by setrlimit\n");
    SET_LIMIT(RLIMIT_FSIZE, 1048576, 1048576);
    SET_LIMIT(RLIMIT_NOFILE, 1024, 2048);
    SET_LIMIT(RLIMIT_STACK, 8388608, 16777216);
    SET_LIMIT(RLIMIT_AS, 67108864, 67108864);

    return EXIT_SUCCESS;
}

static void set_rlimit(int resource, const char *name, rlim_t soft_limit, rlim_t hard_limit)
{
    struct rlimit rlim;

    rlim.rlim_cur = soft_limit;
    rlim.rlim_max = hard_limit;

    printf("Resource: %s\n", name);
    if(setrlimit(resource, &rlim) == -1)
    {
        perror("setrlimit");
    }
    else
    {
        printf("  Soft limit set to: %lld\n", (long long) rlim.rlim_cur);
        printf("  Hard limit set to: %lld\n", (long long) rlim.rlim_max);
    }
}
