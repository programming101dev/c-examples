/*
 * Copyright 2026 D'Arcy Smith
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void convert(const char *str);

#define TOLLERANCE 1e-6L

int main(void)
{
    const char goodNumber[]     = "12345.6789";
    const char leftoverNumber[] = "42.99989hello";
    const char badNumber[]      = "abcde";

    convert(goodNumber);
    convert(leftoverNumber);
    convert(badNumber);

    return EXIT_SUCCESS;
}

static void convert(const char *str)
{
    long double result;
    char       *endptr;
    long double tolerance;

    errno  = 0;
    result = strtold(str, &endptr);

    // Define your desired tolerance
    tolerance = TOLLERANCE;

    // Check for conversion errors
    if((errno == ERANGE && (fabsl(result - HUGE_VALL) < tolerance)) || (errno != 0 && fabsl(result) < tolerance))
    {
        fprintf(stderr, "Error during conversion: %s\n", strerror(errno));
    }

    // Check if the entire string was converted
    if(endptr == str)
    {
        fprintf(stderr, "No digits were found in the input.\n");
    }

    // Check for leftover characters in the string
    if(*endptr != '\0')
    {
        fprintf(stderr, "Extra characters after the number: %s\n", endptr);
    }

    printf("Result: %Lf\n", result);
}
