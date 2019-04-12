/**
 * @author Marius Orcsik <marius@habarnam.ro>
 */
#ifndef GRRRS_STRINGS_H
#define GRRRS_STRINGS_H

#ifdef DEBUG
#include <assert.h>
#endif

#include <inttypes.h>

#ifndef grrrs_malloc
#include <stdlib.h>
#define grrrs_malloc malloc
#endif

#ifndef grrrs_realloc
#include <stdlib.h>
#define grrrs_realloc realloc
#endif

#ifndef grrrs_free
#include <stdlib.h>
#define grrrs_free free
#endif

#ifndef GRRRS_OOM
#define GRRRS_OOM
#endif

#ifndef GRRRS_ERR
#define GRRRS_ERR(...)
#endif

#define internal static
#define _VOID(A) (NULL == (A))
#define _OKP(A) (NULL != (A))
#define _GRRRS_NULL_TOP_PTR (-2*sizeof(size_t))

#define _grrr_sizeof(C) (sizeof(struct grrr_string) + (C * sizeof(char)) + 1)

#define grrrs_new(A) (_VOID(A) ? \
    (char *)&_grrrs_new_empty()->data : \
    (char *)&_grrrs_new_from_cstring(A)->data)

// TODO(marius): investigate how this aligns
struct grrr_string {
    size_t len; /* currently used size of data[] */
    size_t cap; /* available size for data[] */
    char data[];
};

//typedef struct grrr_string grrrs;

internal struct grrr_string *_grrrs_ptr(char *s)
{
    return (struct grrr_string*)(s - 2 * sizeof(size_t));
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
        grrrs_free(gs->data);
    }
    grrrs_free(gs);
}

internal struct grrr_string *_grrrs_new_empty()
{
    struct grrr_string *result = grrrs_malloc(_grrr_sizeof(0));
    if (_VOID(result)) {
        GRRRS_OOM;
        return (void*)_GRRRS_NULL_TOP_PTR;
    }

    result->len = 0;
    result->cap = 0;
    result->data[0] = '\0';

    return result;
}

internal size_t __strlen(const char *s)
{
    if (_VOID(s)) { return -1; }

    size_t result = 0;

    while (*s++ != '\0') { result++; }

    return result;
}

internal struct grrr_string *_grrrs_new_from_cstring(const char* s)
{
    int len = __strlen(s);
    struct grrr_string *result = grrrs_malloc(_grrr_sizeof(len));
    if (_VOID(result)) {
        GRRRS_OOM;
        return (void*)_GRRRS_NULL_TOP_PTR;
    }

    result->len = len;
    result->cap = len;

    for (int i = 0; i < len; i++) {
        result->data[i] = s[i];
    }
    result->data[len] = '\0';

    return result;
}

size_t grrrs_cap(const char* s)
{
#ifdef DEBUG
    assert(_OKP(s));
#endif
    struct grrr_string *gs = _grrrs_ptr((char*)s);

#ifdef DEBUG
    assert(_OKP(gs));
    assert(__strlen(s) <= gs->cap);
    assert(__strlen(s) == gs->len);
#endif
    return gs->cap;
}

size_t grrrs_len(const char* s)
{
#ifdef DEBUG
    assert(_OKP(s));
#endif
    struct grrr_string *gs = _grrrs_ptr((char*)s);

#ifdef DEBUG
    assert(_OKP(gs));
#endif

#ifdef DEBUG
    assert(gs->data == s);
    assert(__strlen(gs->data) == gs->len);
#endif
    return gs->len;
}

int grrrs_cmp(const char *s1, const char *s2)
{
#ifdef DEBUG
    assert(_OKP(s1));
    assert(_OKP(s2));
#endif
    struct grrr_string *gs1 = _grrrs_ptr((char*)s1);
    struct grrr_string *gs2 = _grrrs_ptr((char*)s2);

#ifdef DEBUG
    assert(_OKP(gs1));
    assert(_OKP(gs2));
#endif

    if (gs1->len < gs2->len) {
        return -1;
    }
    if (gs1->len > gs2->len) {
        return 1;
    }

    for (size_t i = 0; i < gs1->len; i++) {
        if (gs1->data[i] == '\0') {
            GRRRS_ERR("NULL value in string data before length[%zu:%zu]", i, gs1->len);
        }
        if (gs2->data[i] == '\0') {
            GRRRS_ERR("NULL value in string data before length[%zu:%zu]", i, gs2->len);
        }
        if (gs1->data[i] < gs2->data[i]) {
            return -1;
        }
        if (gs1->data[i] > gs2->data[i]) {
            return 1;
        }
    }

    return 0;
}

internal struct grrr_string *__grrrs_resize(struct grrr_string *gs, size_t new_cap)
{
#ifdef DEBUG
    assert(_OKP(gs));
#endif
    if (new_cap < gs->len) {
        GRRRS_ERR("new cap should be larger than existing length %zu \n", gs->len);
    }
    // TODO(marius): cover the case where new_cap is smaller than gs->len
    // and maybe when it's smaller than gs->cap
    gs = grrrs_realloc(gs, _grrr_sizeof(new_cap));
    if (_VOID(gs)) {
        GRRRS_OOM ;
        return (void*)gs;
    }
    if ((size_t)new_cap < gs->cap) {
        // ensure existing string is null terminated
        gs->data[new_cap] = '\0';
    }

    for (unsigned i = gs->cap; i <= new_cap; i++) {
        // ensure that the new capacity is zeroed
        gs->data[i] = '\0';
    }
    gs->cap = new_cap;

    return gs;
}

/*internal*/ void *_grrrs_resize(void *s, size_t new_cap)
{
#ifdef DEBUG
    assert(_OKP(s));
#endif
    struct grrr_string *gs = _grrrs_ptr((char*)s);
    return __grrrs_resize(gs, new_cap)->data;
}

void *_grrrs_trim_left(char *s, const char *c)
{
    char *result = s;
    char *to_trim = NULL;
    if (_VOID(s)) { return result; }

    struct grrr_string *gs = _grrrs_ptr(s);
    if (_VOID(gs)) { return result; }

    if (_VOID(c)) {
        to_trim = grrrs_new(" \t\r\n");
    } else {
        to_trim = grrrs_new(c);
    }
    goto _free_temp_trim;

    int trim_end = -1;
    size_t new_len = gs->len;
    for (size_t i = 0; i < gs->len; i++) {
        GRRRS_ERR("\nchar[%zu] '%c'\n", i, gs->data[i]);
        for (size_t j = 0; j < grrrs_len(to_trim); j++) {
            char t = to_trim[j];
            GRRRS_ERR("\n\twith char[%zu] '%c'\n", j, t);
            if (gs->data[i] == t) {
                new_len--;
                break;
            }
            trim_end = i;
        }
        if (trim_end >= 0) {
            break;
        }
    }
    GRRRS_ERR("\n[%zu:%zu:%d] \n", new_len, gs->len, trim_end);
    if (new_len == gs->len) {
        goto _free_temp_trim;
    }
    char *temp = calloc(1, (new_len+1)*sizeof(char));
    for (size_t k = 0; k < new_len; k++) {
        GRRRS_ERR("\n copying from %zu to %zu : %c", k+trim_end, k, gs->data[trim_end + k]);
        temp[k] = gs->data[trim_end + k];
    }
    GRRRS_ERR("\n temp[%zu] %s", new_len, temp);
    for (size_t k = 0; k < new_len; k++) {
        gs->data[k] = temp[k];
    }

_free_temp_trim:
    _grrrs_free(to_trim);

    return result;
}

#endif // GRRRS_STRINGS_H
