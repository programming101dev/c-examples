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


#include <float.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


// Character Types
static void convertChar(char c, char *buffer, size_t bufferSize);

// Integer Types
static void convertInt8(int8_t num, char *buffer, size_t bufferSize);
static void convertUInt8(uint8_t num, char *buffer, size_t bufferSize);
static void convertInt16(int16_t num, char *buffer, size_t bufferSize);
static void convertUInt16(uint16_t num, char *buffer, size_t bufferSize);
static void convertInt32(int32_t num, char *buffer, size_t bufferSize);
static void convertUInt32(uint32_t num, char *buffer, size_t bufferSize);
static void convertInt(int num, char *buffer, size_t bufferSize);
static void convertUInt(unsigned int num, char *buffer, size_t bufferSize);
static void convertLong(long num, char *buffer, size_t bufferSize);
static void convertULong(unsigned long num, char *buffer, size_t bufferSize);
static void convertLongLong(long long num, char *buffer, size_t bufferSize);
static void convertULongLong(unsigned long long num, char *buffer, size_t bufferSize);

// Floating-Point Types
static void convertFloat(float num, char *buffer, size_t bufferSize);
static void convertDouble(double num, char *buffer, size_t bufferSize);
static void convertLongDouble(long double num, char *buffer, size_t bufferSize);

// Pointer Types
static void convertPointer(const void *ptr, char *buffer, size_t bufferSize);

// Size and Offset Types
static void convertSize(size_t num, char *buffer, size_t bufferSize);
static void convertSSize(ssize_t num, char *buffer, size_t bufferSize);
static void convertOff(off_t num, char *buffer, size_t bufferSize);

// Boolean Type
static void convertBool(bool value, char *buffer, size_t bufferSize);

// Process and User ID Types (POSIX-specific)
static void convertPid(pid_t num, char *buffer, size_t bufferSize);
static void convertUid(uid_t num, char *buffer, size_t bufferSize);
static void convertGid(gid_t num, char *buffer, size_t bufferSize);


int main(void)
{
    char good_buffer[50];
    char bad_buffer[2];
    int value;

    // Character Types
    printf("Character Types\n");
    printf("char\n");
    convertChar('A', good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertChar('\0', bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    // Integer Types
    printf("int8_t\n");
    convertInt8(-42, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    printf("uint8_t\n");
    convertUInt8(255, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    printf("int16_t\n");
    convertInt16(-12345, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertInt16(32767, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    printf("uint16_t\n");
    convertUInt16(65535, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    printf("int32_t\n");
    convertInt32(-2147483647 - 1, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertInt32(2147483647, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    printf("uint32_t\n");
    convertUInt32(4294967295U, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    printf("int\n");
    convertInt(-2147483647 - 1, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertInt(2147483647, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    printf("unsigned int\n");
    convertUInt(12345, good_buffer, sizeof(good_buffer)); // Using the function
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertUInt(2147483647, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    printf("long\n");
    convertLong(4294967295L, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertLong(5000000000L, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    printf("unsigned long\n");
    convertULong(4294967295UL, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertULong(5000000000UL, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    printf("long long\n");
    convertLongLong(-9223372036854775807LL - 1LL, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertLongLong(9223372036854775807LL, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    printf("unsigned long long\n");
    convertULongLong(18446744073709551615ULL, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertULongLong(18446744073709551615ULL, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    // Floating-Point Types
    printf("float\n");
    convertFloat(3.14159f, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertFloat(FLT_MAX, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    printf("double\n");
    convertDouble(DBL_MAX, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    printf("long double\n");
    convertLongDouble(3.141592653589793238462643383279502884L, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertLongDouble(9876543210.9876543210L, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    // Boolean Type
    printf("bool\n");
    convertBool(true, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertBool(false, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    // Pointer Types
    printf("\nPointer Types\n");
    value = 42;
    printf("type*\n");
    convertPointer(&value, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertPointer(NULL, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    // Size and Offset Types
    printf("size_t\n");
    convertSize(sizeof(int), good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    printf("ssize_t\n");
    convertSSize(-42, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertSSize(12345, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    printf("off_t\n");
    convertOff(9876543210, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertOff(-1, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    // Process and User ID Types
    printf("pid_t\n");
    convertPid(12345, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertPid(-1, bad_buffer, sizeof(bad_buffer));
    printf("\tBad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    printf("uid_t\n");
    convertUid(1000, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    printf("gid_t\n");
    convertGid(100, good_buffer, sizeof(good_buffer));
    printf("\tGood Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    return EXIT_SUCCESS;
}


static void convertChar(char value, char *buffer, size_t bufferSize)
{
    if (bufferSize >= 7)  // Ensure enough space for "char: X\0"
    {
        snprintf(buffer, bufferSize, "char: %c", value);
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertInt8(int8_t value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "int8_t: %"
    PRId8, value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertUInt8(uint8_t value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "uint8_t: %"
    PRIu8, value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertInt16(int16_t value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "int16_t: %"
    PRId16, value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertUInt16(uint16_t value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "uint16_t: %"
    PRIu16, value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertInt32(int32_t value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "int32_t: %"
    PRId32, value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertUInt32(uint32_t value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "uint32_t: %"
    PRIu32, value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertInt(int value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "int: %d", value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertUInt(unsigned int value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "uint: %u", value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertLong(long value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "long: %ld", value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertULong(unsigned long value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "ulong: %lu", value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertLongLong(long long value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "long long: %lld", value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertULongLong(unsigned long long value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "ulonglong: %llu", value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertFloat(float value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "float: %.5f", (double) value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertDouble(double value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "double: %.10lf", value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertLongDouble(long double value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "long double: %.15Lf", value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertBool(bool value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "bool: %s", value ? "true" : "false");

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertPointer(const void *value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "pointer: %p", value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertSize(size_t value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "size_t: %zu", value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertSSize(ssize_t value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "ssize_t: %zd", value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertOff(off_t value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "off_t: %"
    PRId64, (int64_t) value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertPid(pid_t value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "pid_t: %"
    PRIdMAX, (intmax_t) value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertUid(uid_t value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "uid_t: %"
    PRIdMAX, (intmax_t) value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}


static void convertGid(gid_t value, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "gid_t: %"
    PRIdMAX, (intmax_t) value);

    if(result >= 0 && (size_t) result < bufferSize)
    {
        buffer[result] = '\0';
    }
    else
    {
        snprintf(buffer, bufferSize, "E");
    }
}

