#include <stdio.h>
#include <string.h>


void convertLong(const long num, char *buffer, size_t bufferSize);
void convertLongLong(const long long num, char *buffer, size_t bufferSize);
void convertUnsignedLong(const unsigned long num, char *buffer, size_t bufferSize);
void convertUnsignedLongLong(const unsigned long long num, char *buffer, size_t bufferSize);
void convertFloat(const float num, char *buffer, size_t bufferSize);
void convertDouble(const double num, char *buffer, size_t bufferSize);
void convertLongDouble(const long double num, char *buffer, size_t bufferSize);


int main(void)
{
    char good_buffer[50];
    char bad_buffer[2];

    convertLong(12345L, good_buffer, sizeof(good_buffer));
    printf("Good Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertLong(54321L, bad_buffer, sizeof(bad_buffer));
    printf("Bad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    convertLongLong(1234567890123456789LL, good_buffer, sizeof(good_buffer));
    printf("Good Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertLongLong(1234567890123456789LL, bad_buffer, sizeof(bad_buffer));
    printf("Bad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    convertUnsignedLong(123456UL, good_buffer, sizeof(good_buffer));
    printf("Good Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertUnsignedLong(654321UL, bad_buffer, sizeof(bad_buffer));
    printf("Bad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    convertUnsignedLongLong(12345678901234567890ULL, good_buffer, sizeof(good_buffer));
    printf("Good Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertUnsignedLongLong(9876543210987654321ULL, bad_buffer, sizeof(bad_buffer));
    printf("Bad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    convertFloat(3.14159f, good_buffer, sizeof(good_buffer));
    printf("Good Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertFloat(9876543210.9876543210f, bad_buffer, sizeof(bad_buffer));
    printf("Bad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    convertDouble(3.14159265358979, good_buffer, sizeof(good_buffer));
    printf("Good Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertDouble(9876543210.9876543210, bad_buffer, sizeof(bad_buffer));
    printf("Bad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    convertLongDouble(3.141592653589793238462643383279502884L, good_buffer, sizeof(good_buffer));
    printf("Good Buffer: %s\n", good_buffer);
    memset(good_buffer, 0, sizeof(good_buffer));

    convertLongDouble(9876543210.9876543210L, bad_buffer, sizeof(bad_buffer));
    printf("Bad Buffer: %s\n", bad_buffer);
    memset(bad_buffer, 0, sizeof(bad_buffer));

    return 0;
}

void convertLong(const long num, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "Result: %ld", num);

    if (result >= 0 && (size_t)result < bufferSize) {
        // Conversion successful
        buffer[result] = '\0'; // Null-terminate the string
    } else {
        snprintf(buffer, bufferSize, "E");
    }
}

void convertLongLong(const long long num, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "Result: %lld", num);

    if (result >= 0 && (size_t)result < bufferSize) {
        // Conversion successful
        buffer[result] = '\0'; // Null-terminate the string
    } else {
        snprintf(buffer, bufferSize, "E");
    }
}

void convertUnsignedLong(const unsigned long num, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "Result: %lu", num);

    if (result >= 0 && (size_t)result < bufferSize) {
        // Conversion successful
        buffer[result] = '\0'; // Null-terminate the string
    } else {
        snprintf(buffer, bufferSize, "E");
    }
}

void convertUnsignedLongLong(const unsigned long long num, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "Result: %llu", num);

    if (result >= 0 && (size_t)result < bufferSize) {
        // Conversion successful
        buffer[result] = '\0'; // Null-terminate the string
    } else {
        snprintf(buffer, bufferSize, "E");
    }
}

void convertFloat(const float num, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "Result: %f", num);

    if (result >= 0 && (size_t)result < bufferSize) {
        // Conversion successful
        buffer[result] = '\0'; // Null-terminate the string
    } else {
        snprintf(buffer, bufferSize, "E");
    }
}

void convertDouble(const double num, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "Result: %lf", num);

    if (result >= 0 && (size_t)result < bufferSize) {
        // Conversion successful
        buffer[result] = '\0'; // Null-terminate the string
    } else {
        snprintf(buffer, bufferSize, "E");
    }
}

void convertLongDouble(const long double num, char *buffer, size_t bufferSize)
{
    int result = snprintf(buffer, bufferSize, "Result: %Lf", num);

    if (result >= 0 && (size_t)result < bufferSize) {
        // Conversion successful
        buffer[result] = '\0'; // Null-terminate the string
    } else {
        snprintf(buffer, bufferSize, "E");
    }
}
