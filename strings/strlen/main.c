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

#include <getopt.h>
#include <p101_c/p101_stdio.h>
#include <p101_c/p101_stdlib.h>
#include <p101_c/p101_string.h>
#include <stdio.h>
#include <stdlib.h>

static void           setup(struct p101_error **err, struct p101_env **env, bool report, bool trace);
static void           config(struct p101_error *err, struct p101_env *env, bool report, bool trace);
static void           teardown(struct p101_error *err, struct p101_env *env);
static void           parse_arguments(const struct p101_env *env, struct p101_error *err, int argc, char *argv[], bool *report, bool *trace, char **string);
static void           handle_arguments(const struct p101_env *env, struct p101_error *err, const char *binary_name, const char *string);
_Noreturn static void usage(const struct p101_env *env, struct p101_error *err, const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24

int main(int argc, char *argv[])
{
    struct p101_error *err;
    struct p101_env   *env;
    bool               report;
    bool               trace;
    char              *string;
    size_t             length;

    report = false;
    trace  = false;
    string = NULL;
    setup(&err, &env, report, trace);
    parse_arguments(env, err, argc, argv, &report, &trace, &string);
    config(err, env, report, trace);
    handle_arguments(env, err, argv[0], string);
    length = p101_strlen(env, string);
    printf("Length of \"%s\" is %zu\n", string, length);
    teardown(err, env);

    return EXIT_SUCCESS;
}

static void setup(struct p101_error **err, struct p101_env **env, bool report, bool trace)
{
    *err = p101_error_create(report);
    *env = p101_env_create(*err, true, trace ? p101_env_default_tracer : NULL);
}

static void config(struct p101_error *err, struct p101_env *env, bool report, bool trace)
{
    p101_error_set_reporting(err, report);
    p101_env_set_tracer(env, trace ? p101_env_default_tracer : NULL);
}

static void teardown(struct p101_error *err, struct p101_env *env)
{
    free(env);
    p101_error_reset(err);
    free(err);
}

static void parse_arguments(const struct p101_env *env, struct p101_error *err, int argc, char *argv[], bool *report, bool *trace, char **string)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "hrt")) != -1)
    {
        switch(opt)
        {
            case 'r':
            {
                *report = true;
                break;
            }
            case 't':
            {
                *trace = true;
                break;
            }
            case 'h':
            {
                usage(env, err, argv[0], EXIT_SUCCESS, NULL);
            }
            case '?':
            {
                char message[UNKNOWN_OPTION_MESSAGE_LEN];

                snprintf(message, sizeof(message), "Unknown option '-%c'.", optopt);
                usage(env, err, argv[0], EXIT_FAILURE, message);
            }
            default:
            {
                usage(env, err, argv[0], EXIT_FAILURE, NULL);
            }
        }
    }

    if(optind >= argc)
    {
        usage(env, err, argv[0], EXIT_FAILURE, "The string is required");
    }

    if(optind < argc - 1)
    {
        usage(env, err, argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *string = argv[optind];
}

static void handle_arguments(const struct p101_env *env, struct p101_error *err, const char *binary_name, const char *string)
{
    P101_TRACE(env);

    if(string == NULL)
    {
        usage(env, err, binary_name, EXIT_FAILURE, "The string is required.");
    }
}

_Noreturn static void usage(const struct p101_env *env, struct p101_error *err, const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] [-r] [-t] <string>\n", program_name);
    p101_fputs(env, err, "Options:\n", stderr);
    p101_fputs(env, err, "  -h  Display this help message\n", stderr);
    p101_fputs(env, err, "  -r  Report any errors\n", stderr);
    p101_fputs(env, err, "  -t  Trace function calls\n", stderr);
    p101_exit(env, exit_code);
}
