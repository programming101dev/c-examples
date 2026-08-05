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

#define TOLLERANCE 1e-6f

int main(void)
{
    const char goodNumber[]     = "123.456";
    const char leftoverNumber[] = "42.14234hello";
    const char badNumber[]      = "abcde";

    convert(goodNumber);
    convert(leftoverNumber);
    convert(badNumber);

    return EXIT_SUCCESS;
}

static void convert(const char *str)
{
    float result;
    char *endptr;
    float tolerance;

    errno     = 0;
    result    = strtof(str, &endptr);
    tolerance = TOLLERANCE;    // Define your desired tolerance for float

    if((errno == ERANGE && (fabsf(result - HUGE_VALF) < tolerance || fabsf(result + HUGE_VALF) < tolerance)) || (errno != 0 && fabsf(result) < tolerance))
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

    printf("Result: %f\n", (double)result);
}
