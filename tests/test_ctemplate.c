#include <string.h>

#include "ctemplate/ctemplate.h"
#include "test_framework.h"

TEST_CASE(create_destroy_does_not_crash) {
    ctemplate_strbuf *sb = ctemplate_strbuf_create(0);
    TEST_ASSERT(sb != NULL);
    TEST_ASSERT_EQUAL_INT(0, ctemplate_strbuf_length(sb));
    TEST_ASSERT_EQUAL_STRING("", ctemplate_strbuf_cstr(sb));
    ctemplate_strbuf_destroy(sb);
}

TEST_CASE(append_accumulates_text) {
    ctemplate_strbuf *sb = ctemplate_strbuf_create(4);
    TEST_ASSERT(sb != NULL);

    TEST_ASSERT_EQUAL_INT(CTEMPLATE_OK, ctemplate_strbuf_append(sb, "Hello, "));
    TEST_ASSERT_EQUAL_INT(CTEMPLATE_OK, ctemplate_strbuf_append(sb, "world!"));

    TEST_ASSERT_EQUAL_STRING("Hello, world!", ctemplate_strbuf_cstr(sb));
    TEST_ASSERT_EQUAL_INT((int)strlen("Hello, world!"), (int)ctemplate_strbuf_length(sb));

    ctemplate_strbuf_destroy(sb);
}

TEST_CASE(append_grows_past_initial_capacity) {
    ctemplate_strbuf *sb = ctemplate_strbuf_create(1);
    TEST_ASSERT(sb != NULL);

    for (int i = 0; i < 100; i++) {
        TEST_ASSERT_EQUAL_INT(CTEMPLATE_OK, ctemplate_strbuf_append(sb, "0123456789"));
    }

    TEST_ASSERT_EQUAL_INT(1000, (int)ctemplate_strbuf_length(sb));
    ctemplate_strbuf_destroy(sb);
}

TEST_CASE(clear_resets_length_but_keeps_buffer_usable) {
    ctemplate_strbuf *sb = ctemplate_strbuf_create(0);
    TEST_ASSERT(sb != NULL);

    ctemplate_strbuf_append(sb, "temporary");
    ctemplate_strbuf_clear(sb);

    TEST_ASSERT_EQUAL_INT(0, ctemplate_strbuf_length(sb));
    TEST_ASSERT_EQUAL_STRING("", ctemplate_strbuf_cstr(sb));

    TEST_ASSERT_EQUAL_INT(CTEMPLATE_OK, ctemplate_strbuf_append(sb, "reused"));
    TEST_ASSERT_EQUAL_STRING("reused", ctemplate_strbuf_cstr(sb));

    ctemplate_strbuf_destroy(sb);
}

TEST_CASE(invalid_arguments_return_error_codes) {
    TEST_ASSERT_EQUAL_INT(CTEMPLATE_ERR_INVALID_ARG, ctemplate_strbuf_append(NULL, "x"));

    ctemplate_strbuf *sb = ctemplate_strbuf_create(0);
    TEST_ASSERT_EQUAL_INT(CTEMPLATE_ERR_INVALID_ARG, ctemplate_strbuf_append(sb, NULL));
    ctemplate_strbuf_destroy(sb);

    /* NULL is accepted by destroy/clear/length/cstr, mirroring free(). */
    ctemplate_strbuf_destroy(NULL);
    ctemplate_strbuf_clear(NULL);
    TEST_ASSERT_EQUAL_INT(0, ctemplate_strbuf_length(NULL));
    TEST_ASSERT_EQUAL_STRING("", ctemplate_strbuf_cstr(NULL));
}

TEST_MAIN_BEGIN()
    RUN_TEST(create_destroy_does_not_crash);
    RUN_TEST(append_accumulates_text);
    RUN_TEST(append_grows_past_initial_capacity);
    RUN_TEST(clear_resets_length_but_keeps_buffer_usable);
    RUN_TEST(invalid_arguments_return_error_codes);
TEST_MAIN_END()
