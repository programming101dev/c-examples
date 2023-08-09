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
