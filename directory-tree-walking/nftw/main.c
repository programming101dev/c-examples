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
#include <getopt.h>
#include <ftw.h>


static void display_help(const char *program_name);
static int print_file(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);


int main(int argc, char *argv[])
{
    int opt;
    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch(opt)
        {
            case 'h':
                display_help(argv[0]);
                return 0;
            default:
                display_help(argv[0]);
                return 1;
        }
    }

    if(optind != argc - 1)
    {
        display_help(argv[0]);
        return 1;
    }

    // Use nftw to traverse the directory tree recursively
    if(nftw(argv[optind], print_file, 10, FTW_PHYS) == -1)
    {
        perror("nftw");
        return 1;
    }

    return EXIT_SUCCESS;
}


static void display_help(const char *program_name)
{
    printf("Usage: %s <directory>\n", program_name);
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int print_file(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
    if(tflag == FTW_F)
    {
        printf("File: %s\n", fpath);
    }
    else if(tflag == FTW_D)
    {
        printf("Directory: %s\n", fpath);
    }
    else if(tflag == FTW_SL)
    {
        printf("Link: %s\n", fpath);
    }

    return 0; // Continue traversing the directory tree
}

#pragma GCC diagnostic pop
