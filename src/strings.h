/**
 * @author Marius Orcsik <marius@habarnam.ro>
 */
#ifndef GRRRS_STRINGS_H
#define GRRRS_STRINGS_H

#if DDEBUG
#include <assert.h>
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#ifndef GRRRS_MALLOC
#define GRRRS_MALLOC(...) malloc(__VA_ARGS__)
#endif

#ifndef GRRRS_REALLOC
#define GRRRS_REALLOC(A, ...) realloc((A), __VA_ARGS__)
#endif

#ifndef GRRRS_FREE
#define GRRRS_FREE(A) free(A)
#endif

#ifndef GRRRS_OOM
#define GRRRS_OOM
#endif

#define internal static
#define _VOID(A) (NULL == (A))
#define _OKP(A) (NULL != (A))

// TODO(marius): investigate how this aligns
struct grrr_string {
    int_fast16_t len;
    int_fast16_t cap;
    char data[];
};

//typedef struct grrr_string grrrs;

internal struct grrr_string *_grrrs_ptr(char *s)
{
    return (struct grrr_string*)(s - 2*sizeof(int_fast16_t));
}

void _grrrs_free(char *s)
{
    if (_VOID(s)) { return; }

    struct grrr_string *gs = _grrrs_ptr(s);

    if ((void*)s != (void*)&gs->data) {
        // TODO(marius): Add some more checks to see if memory layout matches.
        return;
    }

    if (_VOID(gs->data)) {
        GRRRS_FREE(gs->data);
    }
    GRRRS_FREE(gs);
}

struct grrr_string *_grrrs_new_empty()
{
    struct grrr_string *result = GRRRS_MALLOC(sizeof(struct grrr_string));
    if (_VOID(result)) {
        GRRRS_OOM;
        return (void*)(2*sizeof(char));
    }

    result->len = 0;
    result->cap = 0;

    return result;
}

internal int __strlen(const char *s)
{
    if (_VOID(s)) { return -1; }

    int result = 0;
    while (*s++ != '\0') { result++; }

    return result;
}

struct grrr_string *_grrrs_new_from_cstring(const char* s)
{
    int len = __strlen(s);
    struct grrr_string *result = GRRRS_MALLOC(sizeof(struct grrr_string) + (len * sizeof(char)) + 1);
    if (_VOID(result)) {
        GRRRS_OOM;
        return (void*)(2*sizeof(char));
    }

    result->len = len;
    result->cap = len;

    for (int i = 0; i < len; i++) {
        result->data[i] = s[i];
    }
    result->data[len] = '\0';

    return result;
}

int grrrs_cap(const char* s)
{
    struct grrr_string *gs = _grrrs_ptr((char*)s);
#if DDEBUG
    assert(_OKP(gs))
    assert(__strlen(s) <= gs->cap);
#endif
    return gs->cap;
}


int grrrs_len(const char* s)
{
    struct grrr_string *gs = _grrrs_ptr((char*)s);
#if DDEBUG
    assert(_OKP(gs))
    assert(__strlen(s) == gs->len);
#endif
    return gs->len;
}

void *_grrrs_resize(void *s, int new_cap)
{
    struct grrr_string *gs = _grrrs_ptr((char*)s);
#if DDEBUG
    assert(_OKP(gs))
#endif
    gs = realloc(gs, sizeof(struct grrr_string) + (new_cap * sizeof(char)) + 1);

    return gs->data;
}

#define grrrs_new(A) (_VOID(A) ? \
    (char *)&_grrrs_new_empty()->data : \
    (char *)&_grrrs_new_from_cstring(A)->data)

#endif // GRRRS_STRINGS_H
