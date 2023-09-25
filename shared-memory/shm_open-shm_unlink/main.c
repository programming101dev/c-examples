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


#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **shm_name);
static void handle_arguments(const char *binary_name, const char *shm_name);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


#define UNKNOWN_OPTION_MESSAGE_LEN 24


int main(int argc, char *argv[])
{
    char *shm_name;
    int  shm_fd;

    shm_name = NULL;
    parse_arguments(argc, argv, &shm_name);
    handle_arguments(argv[0], shm_name);
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if(shm_fd == -1)
    {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    printf("Opened %s\n", shm_name);
    close(shm_fd);
    printf("Closed %s\n", shm_name);

    // Unlink the shared memory object
    if(shm_unlink(shm_name) == -1)
    {
        perror("shm_unlink");
        return EXIT_FAILURE;
    }

    printf("Unlinked %s\n", shm_name);

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **shm_name)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "h:")) != -1)
    {
        switch(opt)
        {
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
            }
            case '?':
            {
                char message[UNKNOWN_OPTION_MESSAGE_LEN];
                snprintf(message, sizeof(message), "Unknown option '-%c'.", optopt);
                usage(argv[0], EXIT_FAILURE, message);
            }
            default:
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
            }
        }
    }

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The library name is required");
    }
    else if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *shm_name = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *shm_name)
{
    if(shm_name == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The shared memory name is required.");
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <shared memory name>\n", program_name);
    exit(exit_code);
}
