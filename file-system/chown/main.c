#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


static void usage(const char *program_name, int exit_code);


int main(int argc, char *argv[])
{
    const char *file_path = NULL;
    uid_t user_id = 0;
    gid_t group_id = 0;
    int opt;

    while((opt = getopt(argc, argv, "hf:u:g:")) != -1)
    {
        switch(opt)
        {
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS);
            }
            case 'f':
            {
                file_path = optarg;
                break;
            }
            case 'u':
            {
                errno = 0;
                user_id = strtol(optarg, NULL, 10);

                if(errno != 0)
                {
                    perror("Invalid user ID");
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case 'g':
            {
                errno = 0;
                group_id = strtol(optarg, NULL, 10);

                if(errno != 0)
                {
                    perror("Invalid group ID");
                    exit(EXIT_FAILURE);
                }
                break;
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE);
            }
        }
    }

    if(file_path == NULL || user_id == 0 || group_id == 0)
    {
        fprintf(stderr, "Usage: %s -f <file_path> -u <user_id> -g <group_id>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(chown(file_path, user_id, group_id) == -1)
    {
        perror("chown");
        exit(EXIT_FAILURE);
    }

    printf("File ownership changed successfully.\n");

    exit(EXIT_SUCCESS);
}

static void usage(const char *program_name, int exit_code)
{
    printf("Usage: %s -f <file_path> -u <user_id> -g <group_id>\n", program_name);
    printf("Options:\n");
    printf("  -h : Display this help message\n");
    printf("  -f : File path\n");
    printf("  -u : User ID\n");
    printf("  -g : Group ID\n");
    exit(exit_code);
}
