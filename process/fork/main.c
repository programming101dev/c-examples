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
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>


static void print_process_info(const char *name);
static void print_help(const char *program_name);


int main(int argc, char *argv[])
{
    int opt;
    bool sleep_option;

    sleep_option = false;

    // Parse command-line options
    while((opt = getopt(argc, argv, "sh")) != -1)
    {
        switch(opt)
        {
            case 's':
                sleep_option = true;
                break;
            case 'h':
                print_help(argv[0]);
                return EXIT_SUCCESS;
            case '?':
                fprintf(stderr, "Unknown option: '%c'\n", optopt);
                print_help(argv[0]);
                return EXIT_FAILURE;
            default:
                break;
        }
    }

    pid_t pid = fork();

    if(pid == -1)
    {
        perror("Error creating child process");
        return EXIT_FAILURE;
    }
    else if(pid == 0)
    {
        // This is the child process
        print_process_info("Child");
        printf("Child process finished.\n");
    }
    else
    {
        // This is the parent process
        print_process_info("Parent");

        if(sleep_option)
        {
            sleep(2);
        }

        printf("Parent process finished.\n");
    }

    return EXIT_SUCCESS;
}

static void print_process_info(const char *name)
{
    printf("Name: %s, Process: PID=%d, Parent PID=%d\n", name, getpid(), getppid());
}

static void print_help(const char *program_name)
{
    printf("Usage: %s [-s] [-h]\n", program_name);
    printf("Options:\n");
    printf("  -s  Sleep for 2 seconds (only applicable for the parent process)\n");
    printf("  -h  Display this help message\n");
}
