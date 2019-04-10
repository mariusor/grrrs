/**
 * @author Marius Orcsik <marius@habarnam.ro>
 */

#include <stdio.h>
#define GRRRS_ERR(...) fprintf(stderr, __VA_ARGS__)
#define GRRRS_OOM assert(!"Failed to allocate memory")

#include "strings.h"

#include <snow/snow.h>

void assert_grrrs(char *t) {
    struct grrr_string *gs = _grrrs_ptr(t);
    asserteq_ptr((void*)&gs->data, (void*)t);
    asserteq_ptr((void*)&gs->cap, (void*)(t - sizeof(gs->cap)));
    asserteq_ptr((void*)&gs->len, (void*)(t - sizeof(gs->cap) - sizeof(gs->len)));
    asserteq_ptr((void*)gs, (void*)(t - sizeof(gs->cap) - sizeof(gs->len)));
    asserteq_int(gs->len, strlen(t));
    asserteq_int(gs->cap, strlen(t));
}

describe(basic) {
    subdesc(plumbing) {
        it ("Check __strlen") {
            const char *t = "ana are mere";

            asserteq_int(strlen(t), __strlen(t));
        }
    }
    subdesc(pointer plumbing) {
        it("Check pointer addresses") {
            char *t = grrrs_new(0);

            asserteq_int(grrrs_len(t), 0);
            asserteq_int(grrrs_cap(t), 0);
            asserteq_buf(t, "", 1);

            assert_grrrs(t);

            _grrrs_free(t);
        }
    }

    subdesc(initialiaztion) {
        it("Zero string") {
            char *t = grrrs_new(0);

            asserteq_int(grrrs_len(t), 0);
            asserteq_int(grrrs_cap(t), 0);
            asserteq_buf(t, "", 1);
            _grrrs_free(t);
        }

        it("From C string") {
            char *t = grrrs_new("ana are mere\n");
            asserteq_int(grrrs_len(t), 13);
            asserteq_int(grrrs_cap(t), 13);
            asserteq_buf(t, "ana are mere\n", 14, "Different strings");

            assert_grrrs(t);

            _grrrs_free(t);
        }
    }

    subdesc(operations) {
        it("Grow string") {
            char *t = grrrs_new(0);
            asserteq_int(grrrs_len(t), 0);
            asserteq_int(grrrs_cap(t), 0);

            t = _grrrs_resize(t, 128);
            asserteq_int(grrrs_cap(t), 128);
            asserteq_int(grrrs_len(t), 0);

            char null_block[128] = {0};

            asserteq_int(memcmp(t, &null_block, 128), 0);

            _grrrs_free(t);
        }
        it("Shrink string") {
            char *t = grrrs_new(0);
            asserteq_int(grrrs_len(t), 0);
            asserteq_int(grrrs_cap(t), 0);

            t = _grrrs_resize(t, 128);
            asserteq_int(grrrs_cap(t), 128);
            asserteq_int(grrrs_len(t), 0);

            {
                char null_block[128] = {0};

                asserteq_int(memcmp(t, &null_block, 128), 0);
            }

            t = _grrrs_resize(t, 1);
            asserteq_int(grrrs_cap(t), 1);
            asserteq_int(grrrs_len(t), 0);

            {
                char null_block[1] = {0};

                asserteq_int(memcmp(t, &null_block, 1), 0);
            }

            _grrrs_free(t);
        }
    }
}
snow_main();
