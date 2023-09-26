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


struct func_info
{
    int (*func)(int ch);
    const char *name;
    const char *message;
};


static void print_binary(unsigned int ch);
static void print_info(const struct func_info *info, int ch);


#define WIDTH 6


int main(void)
{
    static struct func_info info[] =
    {
        { isalnum,  "alnum",  "Alphanumeric" },
        { isalpha,  "alpha",  "Alphabetic"   },
        { isblank,  "blank",  "Blank"        },
        { iscntrl,  "cntrl",  "Control"      },
        { isdigit,  "digit",  "Digit"        },
        { isgraph,  "graph",  "Graph"        },
        { islower,  "lower",  "Lowercase"    },
        { isprint,  "print",  "Printable"    },
        { ispunct,  "punct",  "Punctuation"  },
        { isspace,  "space",  "Whitespace"   },
        { isupper,  "upper",  "Uppercase"    },
        { isxdigit, "xdigit", "Hex Digit"    }
    };

    printf("Char | Binary  | Oct | Dec | Hex | ");

    for(size_t i = 0; i < sizeof(info) / sizeof(info[0]); i++)
    {
        printf("%-6s | ", info[i].name);
    }

    printf("Lower | Upper |\n");
    // TODO: this should be dynamic
    printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for(unsigned int ch = 0; ch <= INT8_MAX; ch++)
    {
        char c;
        char lower;
        char upper;

        if(isprint((int)ch))
        {
            c = (char)ch;
        }
        else
        {
            c = ' ';
        }

        printf("%c    | ", c);
        print_binary(ch);
        printf(" | %3o | %3u | %3X | ", ch, ch, ch);

        for(size_t i = 0; i < sizeof(info) / sizeof(info[0]); i++)
        {
            print_info(&info[i], (int)ch);
        }

        if(isprint((int)ch))
        {
            lower = (char)tolower((int)ch);
            upper = (char)toupper((int)ch);
        }
        else
        {
            lower = ' ';
            upper = ' ';
        }

        printf("%c     | %c     |\n", lower, upper);
    }

    return EXIT_SUCCESS;
}


static void print_binary(unsigned int ch)
{
    for(unsigned int i = WIDTH; i != 0; i--)
    {
        printf("%u", (ch >> i) & (unsigned int)1);
    }
}


static void print_info(const struct func_info *info, int ch)
{
    int value;

    value = info->func(ch);
    printf("   %d   | ", value == 0 ? 0 : 1);
}
