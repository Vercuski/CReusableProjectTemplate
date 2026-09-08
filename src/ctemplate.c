#include "ctemplate/ctemplate.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CTEMPLATE_DEFAULT_CAPACITY 32U

struct ctemplate_strbuf {
    char *data;
    size_t length;
    size_t capacity;
};

/* Grow sb's backing storage so it can hold at least min_capacity bytes.
 * Returns 1 on success, 0 if allocation failed (sb is left unchanged). */
static int strbuf_reserve(ctemplate_strbuf *sb, size_t min_capacity) {
    if (min_capacity <= sb->capacity) {
        return 1;
    }

    size_t new_capacity = sb->capacity == 0 ? CTEMPLATE_DEFAULT_CAPACITY : sb->capacity;
    while (new_capacity < min_capacity) {
        if (new_capacity > (SIZE_MAX / 2)) {
            new_capacity = min_capacity;
            break;
        }
        new_capacity *= 2;
    }

    char *new_data = (char *)realloc(sb->data, new_capacity);
    if (new_data == NULL) {
        return 0;
    }

    sb->data = new_data;
    sb->capacity = new_capacity;
    return 1;
}

ctemplate_strbuf *ctemplate_strbuf_create(size_t initial_capacity) {
    ctemplate_strbuf *sb = (ctemplate_strbuf *)malloc(sizeof(*sb));
    if (sb == NULL) {
        return NULL;
    }

    sb->data = NULL;
    sb->length = 0;
    sb->capacity = 0;

    size_t capacity = initial_capacity == 0 ? CTEMPLATE_DEFAULT_CAPACITY : initial_capacity;
    if (!strbuf_reserve(sb, capacity)) {
        free(sb);
        return NULL;
    }

    sb->data[0] = '\0';
    return sb;
}

void ctemplate_strbuf_destroy(ctemplate_strbuf *sb) {
    if (sb == NULL) {
        return;
    }
    free(sb->data);
    free(sb);
}

ctemplate_status ctemplate_strbuf_append(ctemplate_strbuf *sb, const char *text) {
    if (sb == NULL || text == NULL) {
        return CTEMPLATE_ERR_INVALID_ARG;
    }

    size_t text_len = strlen(text);
    if (text_len == 0) {
        return CTEMPLATE_OK;
    }

    size_t required = sb->length + text_len + 1;
    if (!strbuf_reserve(sb, required)) {
        return CTEMPLATE_ERR_NOMEM;
    }

    memcpy(sb->data + sb->length, text, text_len + 1);
    sb->length += text_len;
    return CTEMPLATE_OK;
}

void ctemplate_strbuf_clear(ctemplate_strbuf *sb) {
    if (sb == NULL) {
        return;
    }
    sb->length = 0;
    if (sb->data != NULL) {
        sb->data[0] = '\0';
    }
}

const char *ctemplate_strbuf_cstr(const ctemplate_strbuf *sb) {
    if (sb == NULL || sb->data == NULL) {
        return "";
    }
    return sb->data;
}

size_t ctemplate_strbuf_length(const ctemplate_strbuf *sb) {
    return sb == NULL ? 0 : sb->length;
}
