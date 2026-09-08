#include <stdio.h>

#include "ctemplate/ctemplate.h"

int main(void) {
    ctemplate_strbuf *sb = ctemplate_strbuf_create(0);
    if (sb == NULL) {
        fprintf(stderr, "failed to allocate string buffer\n");
        return 1;
    }

    ctemplate_strbuf_append(sb, "Hello");
    ctemplate_strbuf_append(sb, ", ");
    ctemplate_strbuf_append(sb, "ctemplate!");

    printf("%s\n", ctemplate_strbuf_cstr(sb));
    printf("length = %zu\n", ctemplate_strbuf_length(sb));

    ctemplate_strbuf_destroy(sb);
    return 0;
}
