#ifndef CTEMPLATE_TEST_FRAMEWORK_H
#define CTEMPLATE_TEST_FRAMEWORK_H

/**
 * Minimal, dependency-free test helpers for this template.
 *
 * Real projects usually reach for Unity, Criterion, or CMocka instead; this
 * header exists so the template's tests build and run with nothing beyond a
 * C compiler and CTest. Swap it out freely once you have real dependencies.
 */

#include <stdio.h>
#include <string.h>

static int ctemplate_test_failures = 0;
static int ctemplate_test_count = 0;

#define TEST_CASE(name) static void name(void)

#define RUN_TEST(name)                                                                             \
    do {                                                                                           \
        ctemplate_test_count++;                                                                    \
        fprintf(stderr, "[ RUN      ] %s\n", #name);                                               \
        name();                                                                                    \
        fprintf(stderr, "[       OK ] %s\n", #name);                                               \
    } while (0)

#define TEST_ASSERT(cond)                                                                          \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            fprintf(stderr, "%s:%d: assertion failed: %s\n", __FILE__, __LINE__, #cond);           \
            ctemplate_test_failures++;                                                             \
        }                                                                                          \
    } while (0)

#define TEST_ASSERT_EQUAL_INT(expected, actual)                                                    \
    do {                                                                                           \
        long long exp_ = (long long)(expected);                                                    \
        long long act_ = (long long)(actual);                                                      \
        if (exp_ != act_) {                                                                        \
            fprintf(stderr, "%s:%d: expected %lld, got %lld\n", __FILE__, __LINE__, exp_, act_);   \
            ctemplate_test_failures++;                                                             \
        }                                                                                          \
    } while (0)

#define TEST_ASSERT_EQUAL_STRING(expected, actual)                                                 \
    do {                                                                                           \
        const char *exp_ = (expected);                                                             \
        const char *act_ = (actual);                                                               \
        if (strcmp(exp_, act_) != 0) {                                                             \
            fprintf(stderr, "%s:%d: expected \"%s\", got \"%s\"\n", __FILE__, __LINE__, exp_,      \
                    act_);                                                                         \
            ctemplate_test_failures++;                                                             \
        }                                                                                          \
    } while (0)

#define TEST_MAIN_BEGIN() int main(void) {

#define TEST_MAIN_END()                                                                            \
    fprintf(stderr, "%d test(s), %d failure(s)\n", ctemplate_test_count, ctemplate_test_failures); \
    return ctemplate_test_failures == 0 ? 0 : 1;                                                   \
    }

#endif /* CTEMPLATE_TEST_FRAMEWORK_H */
