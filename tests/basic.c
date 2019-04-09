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
        //asserteq(t, "");
        _grrrs_free(t);
    }

    it("From C string") {
        char *t = grrrs_new("ana are mere\n");
        assert(grrrs_len(t) == 13);
        assert(grrrs_cap(t) == 13);
        //asserteq(t, "ana are mere\n");
        _grrrs_free(t);
    }

    it("Rezize string") {
        char *t = grrrs_new(0);
        assert(grrrs_len(t) == 0);
        assert(grrrs_cap(t) == 0);

        t = _grrrs_resize(t, 128);
        assert(grrrs_cap(t) == 128);
        _grrrs_free(t);
    }
}
snow_main();
