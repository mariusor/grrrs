/**
 * @author Marius Orcsik <marius@habarnam.ro>
 */

#include <stdio.h>
#define GRRRS_ERR(...) fprintf(stderr, __VA_ARGS__)
#define GRRRS_OOM assert(!"Failed to allocate memory")

#include "strings.h"

#include <snow/snow.h>

describe(basic) {
    it("Zero string") {
        char *t = grrrs_new(0);

        assert(grrrs_len(t) == 0);
        assert(grrrs_cap(t) == 0);
        asserteq_buf(t, "", 1, "Different strings");
        _grrrs_free(t);
    }

    it("Check pointer addresses") {
        char *t = grrrs_new(0);

        assert(grrrs_len(t) == 0);
        assert(grrrs_cap(t) == 0);
        asserteq_buf(t, "", 1, "Different strings");
        struct grrr_string *gs = _grrrs_ptr(t);

        assert((void*)&gs->data == (void*)t);
        assert((void*)&gs->cap == (void*)(t - sizeof(gs->cap)));
        assert((void*)&gs->len == (void*)(t - sizeof(gs->cap) - sizeof(gs->len)));
        assert((void*)gs == (void*)(t - sizeof(gs->cap) - sizeof(gs->len)));

        assert(gs->len == 0);
        assert(gs->cap == 0);

        _grrrs_free(t);
    }

    it("From C string") {
        char *t = grrrs_new("ana are mere\n");
        assert(grrrs_len(t) == 13);
        assert(grrrs_cap(t) == 13);
        asserteq_buf(t, "ana are mere\n", 14, "Different strings");
        _grrrs_free(t);
    }

    it("Resize string") {
        char *t = grrrs_new(0);
        assert(grrrs_len(t) == 0);
        assert(grrrs_cap(t) == 0);

        t = _grrrs_resize(t, 128);
        assert(grrrs_cap(t) == 128);
        _grrrs_free(t);
    }
}
snow_main();
