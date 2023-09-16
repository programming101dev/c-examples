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
#include <grp.h>
#include <getopt.h>


static void parse_arguments(int argc, char *argv[], char **group_name);
static void handle_arguments(const char *binary_name, const char *group_name);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void print_entry(const struct group *entry);


int main(int argc, char *argv[])
{
    char *group_name;
    struct group *group_info;

    group_name = NULL;
    parse_arguments(argc, argv, &group_name);
    handle_arguments(argv[0], group_name);
    group_info = getgrnam(group_name);

    if(group_info != NULL)
    {
        print_entry(group_info);
    }
    else
    {
        printf("Group '%s' not found.\n", group_name);
    }

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **group_name)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch(opt)
        {
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
            }
            case '?':
            {
                char message[24];

                snprintf(message, sizeof(message), "Unknown option '-%c'.", optopt);
                usage(argv[0], EXIT_FAILURE, message);
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The group name is required");
    }
    else if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *group_name = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *group_name)
{
    if(group_name == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The group name is required.");
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <group name>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}


static void print_entry(const struct group *entry)
{
    printf("Group name: %s\n", entry->gr_name);
    printf("Group ID (GID): %u\n", entry->gr_gid);
    printf("Group Members:\n");

    if(entry->gr_mem != NULL)
    {
        for(int i = 0; entry->gr_mem[i] != NULL; i++)
        {
            printf(" - %s\n", entry->gr_mem[i]);
        }

        printf("-------------------------\n");
    }
    else
    {
        printf("No members found.\n");
    }
}
