/**
 * @author Marius Orcsik <marius@habarnam.ro>
 */

#include <stdio.h>
#ifndef NO_GRRRS_ERR
#define GRRRS_ERR(...) fprintf(stderr, __VA_ARGS__)
#endif
#define GRRRS_OOM assert("Failed to allocate memory")
#define grrrs_malloc(...) (NULL)
#define grrrs_realloc(...) (NULL)

#include "strings.h"
#include <snow/snow.h>

describe(OOM) {
    it ("Check new with OOM") {
        char *t = grrrs_new("test");
        asserteq_ptr(t, NULL);

        _grrrs_free(t);
    }

    it ("Check grow with OOM") {
        struct grrr_string *t1 = calloc(1, _grrr_sizeof(2)); // allocating for 2 chars
        defer (
            free(t1);
        );
        assertneq_ptr(t1, NULL);
        assertneq_ptr(t1->data, NULL);
        char *t = t1->data;

        t = _grrrs_resize(t, 128);
        asserteq_ptr(t, NULL);
    }
}

snow_main();
