#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>


#define SHOW_LIMIT(resource) \
    show_limit(resource, #resource)


static void show_limit(int resource, const char* name);


int main(void)
{
    SHOW_LIMIT(RLIMIT_CORE);
    SHOW_LIMIT(RLIMIT_CPU);
    SHOW_LIMIT(RLIMIT_DATA);
    SHOW_LIMIT(RLIMIT_FSIZE);
    SHOW_LIMIT(RLIMIT_NOFILE);
    SHOW_LIMIT(RLIMIT_STACK);
    SHOW_LIMIT(RLIMIT_AS);

    return EXIT_SUCCESS;
}

static void show_limit(int resource, const char* name)
{
    struct rlimit rlim;

    if(getrlimit(resource, &rlim) == 0)
    {
        printf("%s:\n", name);
        printf("\tCurrent soft limit: ");

        if (rlim.rlim_cur == RLIM_INFINITY)
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

