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

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct func_info
{
    int (*func)(int ch);
    const char *name;
    void (*print_info)(const struct func_info *info, int ch);
};

static void print_binary(unsigned int ch, unsigned int width);
static void print_info_bool(const struct func_info *info, int ch);
static void print_info_char(const struct func_info *info, int ch);

#define FORMAT_WIDTH 32
#define BINARY_DIGITS 6

int main(void)
{
    static struct func_info info[] = {
        {isalnum,  "alnum",  print_info_bool},
        {isalpha,  "alpha",  print_info_bool},
        {isblank,  "blank",  print_info_bool},
        {iscntrl,  "cntrl",  print_info_bool},
        {isdigit,  "digit",  print_info_bool},
        {isgraph,  "graph",  print_info_bool},
        {islower,  "lower",  print_info_bool},
        {isprint,  "print",  print_info_bool},
        {ispunct,  "punct",  print_info_bool},
        {isspace,  "space",  print_info_bool},
        {isupper,  "upper",  print_info_bool},
        {isxdigit, "xdigit", print_info_bool},
        {toupper,  "upper",  print_info_char},
        {tolower,  "lower",  print_info_char},
    };
    int total_width;

    total_width = printf("Char | Binary | Oct | Dec | Hex | ");

    for(size_t i = 0; i < sizeof(info) / sizeof(info[0]); i++)
    {
        size_t width;
        char   format[FORMAT_WIDTH];

        width = strlen(info[i].name);
        snprintf(format, sizeof(format), "%%-%zus | ", width);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
        total_width += printf(format, info[i].name);
#pragma GCC diagnostic pop
    }

    printf("\n");

    for(int i = 0; i < total_width; i++)
    {
        printf("-");
    }

    printf("\n");

    for(unsigned int ch = 0; ch <= INT8_MAX; ch++)
    {
        char c;

        if(isprint((int)ch))
        {
            c = (char)ch;
        }
        else
        {
            c = ' ';
        }

        printf("%c    | ", c);
        print_binary(ch, BINARY_DIGITS);
        printf(" | %3o | %3u | %3X |", ch, ch, ch);

        for(size_t i = 0; i < sizeof(info) / sizeof(info[0]); i++)
        {
            info[i].print_info(&info[i], (int)ch);
        }

        printf("\n");
    }

    return EXIT_SUCCESS;
}

static void print_binary(unsigned int ch, unsigned int width)
{
    for(unsigned int i = width; i != 0; i--)
    {
        printf("%u", (ch >> i) & (unsigned int)1);
    }
}

static void print_info_bool(const struct func_info *info, int ch)
{
    int    value;
    size_t num_spaces;

    num_spaces = strlen(info->name) + 1;
    value      = info->func(ch) ? 1 : 0;
    printf("%*d |", (int)num_spaces, value);
}

static void print_info_char(const struct func_info *info, int ch)
{
    int    value;
    size_t num_spaces;

    num_spaces = strlen(info->name) + 1;
    value      = info->func(ch);

    if(isspace(value) || iscntrl(value))
    {
        value = ' ';
    }
    else if(!(isprint(value)))
    {
        num_spaces++;
    }

    printf("%*c |", (int)num_spaces, (char)value);
}
