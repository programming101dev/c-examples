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


// TODO take in a string on the command like and print that instead of hard coding


int main(void)
{
    int age;
    double height;

    age = 30;
    height = 1.75;
    fprintf(stdout, "My age is %d and my height is %.2f meters.\n", age, height);
    fprintf(stderr, "Error: Invalid input. Expected an integer, but got '%s'.\n", "xyz");

    return EXIT_SUCCESS;
}
