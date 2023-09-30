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

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static void           parse_arguments(int argc, char *argv[], char **speed);
static void           handle_arguments(const char *binary_name, const char *speed_str, speed_t *speed);
static speed_t        parse_baud_rate(const char *binary_name, const char *baud_rate_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10

// TODO the speed is always invalid

int main(int argc, char *argv[])
{
    char          *speed_str;
    speed_t        input_baud_rate;
    speed_t        new_input_baud_rate;
    struct termios term;

    speed_str = NULL;
    parse_arguments(argc, argv, &speed_str);
    handle_arguments(argv[0], speed_str, &new_input_baud_rate);

    // Get current terminal settings
    if(tcgetattr(STDIN_FILENO, &term) != 0)
    {
        perror("tcgetattr failed");
        return EXIT_FAILURE;
    }

    // Get current input baud rate
    input_baud_rate = cfgetispeed(&term);
    printf("Current input baud rate: %d\n", (int)input_baud_rate);

    if(cfsetispeed(&term, new_input_baud_rate) != 0)
    {
        perror("cfsetispeed failed");
        return EXIT_FAILURE;
    }

    // Update terminal settings
    if(tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0)
    {
        perror("tcsetattr failed");
        return EXIT_FAILURE;
    }

    // Get updated input baud rate
    input_baud_rate = cfgetispeed(&term);
    printf("Updated input baud rate: %d\n", (int)input_baud_rate);

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **speed)
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
                char message[UNKNOWN_OPTION_MESSAGE_LEN];

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
        usage(argv[0], EXIT_FAILURE, "The speed is required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *speed = argv[optind];
}

static void handle_arguments(const char *binary_name, const char *speed_str, speed_t *speed)
{
    if(speed_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The speed is required.");
    }

    *speed = parse_baud_rate(binary_name, speed_str);
}

static speed_t parse_baud_rate(const char *binary_name, const char *baud_rate_str)
{
    static const speed_t baud_rates[] = {
        B0,
        B50,
        B75,
        B110,
        B134,
        B150,
        B200,
        B300,
        B600,
        B1200,
        B1800,
        B2400,
        B4800,
        B9600,
        B19200,
        B38400,
    };
    char         *endptr;
    long long int parsed_speed;
    int           valid_baud_rate;

    errno        = 0;
    parsed_speed = strtoll(baud_rate_str, &endptr, BASE_TEN);

    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing baud rate.");
    }

    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    // Ensure parsed_speed is non-negative and can be safely cast to speed_t
    if(parsed_speed < 0 || parsed_speed > (long long int)B38400)
    {
        usage(binary_name, EXIT_FAILURE, "Invalid baud rate.");
    }

    valid_baud_rate = 0;

    for(size_t i = 0; i < sizeof(baud_rates) / sizeof(baud_rates[0]); i++)
    {
        if((speed_t)parsed_speed == baud_rates[i])
        {
            valid_baud_rate = 1;
            break;
        }
    }

    if(!valid_baud_rate)
    {
        usage(binary_name, EXIT_FAILURE, "Invalid baud rate.");
    }

    return (speed_t)parsed_speed;
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <speed>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}
