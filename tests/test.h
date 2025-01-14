#ifndef CRYSTAL_TEST_H
#define CRYSTAL_TEST_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RESET "\033[0m"

static int failed_assertions = 0;

#define TEST(name) void test_##name(void)
#define RUN_TEST(name)                      \
    do                                      \
    {                                       \
        printf("Running test %s\n", #name); \
        test_##name();                      \
    } while (0)

#define ASSERT(condition)                                                         \
    do                                                                            \
    {                                                                             \
        if (!(condition))                                                         \
        {                                                                         \
            printf(COLOR_RED "Assertion failed: %s\n" COLOR_RESET, #condition);   \
        }                                                                         \
        else                                                                      \
        {                                                                         \
            printf(COLOR_GREEN "Assertion passed: %s\n" COLOR_RESET, #condition); \
        }                                                                         \
                                                                                  \
    } while (0)
#define ASSERT_EQ_STR(expected, actual)                                                                       \
    do                                                                                                        \
    {                                                                                                         \
        if (strcmp(expected, actual) != 0)                                                                    \
        {                                                                                                     \
            printf(COLOR_RED "Assertion Failed:\nEXPECTED: %s\nACTUAL: %s\n" COLOR_RESET, expected, actual);  \
        }                                                                                                     \
        else                                                                                                  \
        {                                                                                                     \
            printf(COLOR_GREEN "Assertion passed: expected %s\nACTUAL: %s \n" COLOR_RESET, expected, actual); \
        }                                                                                                     \
    } while (0)
#endif
