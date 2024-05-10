#include <memory.h>
#include <assert.h>
#include <stdio.h>

#define Array(name) Array_##name

#define ARRAY_DECL(T) \
    typedef struct { \
        T* data; \
        size_t len; \
        size_t cap; \
    } Array_##T; \
\
Array_##T arr_ ##T ##_init(void) { \
    return (Array_ ## T){ \
        .data = malloc(8 * sizeof(T)), \
        .len = 0, \
        .cap = 8, \
    };\
} \
\
void arr_ ##T ##_deinit(Array_ ##T arr) { \
    free(arr.data); \
} \
\
void arr_ ##T ##_append(Array_##T * arr, T item) { \
    if (arr->len >= arr->cap) { \
        arr->cap *= 2; \
        arr->data = realloc(arr->data, arr->cap * sizeof(T)); \
    } \
    assert(arr->len < arr->cap); \
    arr->data[arr->len] = item; \
    arr->len +=1; \
    return; \
} \
\
void arr_ ##T ##_print (Array_ ##T * arr, void(*print_elem)(T)) { \
    printf("["); \
    for (size_t i = 0; i < arr->len; i++) { \
        print_elem(arr->data[i]); \
        printf(", "); \
    } \
    printf("len: %zu, cap: %zu", arr->len, arr->cap); \
    printf("]\n"); \
} \
\
Array_##T arr_##T ##_copy(Array_ ##T * arr) { \
    Array_ ##T result = (Array_ ##T) { \
        .data = malloc(arr->cap * sizeof(T)), \
        .len = arr->len, \
        .cap = arr->cap, \
    }; \
    memcpy(result.data, arr->data, arr->len * sizeof(T)); \
    return result; \
} \
