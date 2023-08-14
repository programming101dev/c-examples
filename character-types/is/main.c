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
#include <stdio.h>
#include <stdlib.h>


static void print_binary(int ch);


int main(void)
{
    printf("Char | Binary  | Oct | Dec | Hex | alnum | alpha | blank | cntrl | digit | graph | lower | print | punct | space | upper | xdigit | lower | upper |\n");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for(int ch = 0; ch <= 127; ch++)
    {
        printf("%c    | ", isprint(ch) ? ch : ' ');
        print_binary(ch);
        printf(" | %3o | %3d | %3X |   %d   |   %d   |   %d   |   %d   |   %d   |   %d   |   %d   |   %d   |   %d   |   %d   |   %d   |   %d    |   %c   |   %c   |\n",
               ch,
               ch,
               ch,
               isalnum(ch),
               isalpha(ch),
               isblank(ch),
               iscntrl(ch),
               isdigit(ch),
               isgraph(ch),
               islower(ch),
               isprint(ch),
               ispunct(ch),
               isspace(ch),
               isupper(ch),
               isxdigit(ch),
               islower(ch) ? tolower(ch) : ' ',   // Print lowercase character for tolower, otherwise print space.
               isupper(ch) ? toupper(ch) : ' '    // Print uppercase character for toupper, otherwise print space.
        );
    }

    return EXIT_SUCCESS;
}

static void print_binary(int ch)
{
    for(int i = 6; i >= 0; i--)
    {
        printf("%d", (ch >> i) & 1);
    }
}
