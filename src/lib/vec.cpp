
#include "vec.hpp"

typedef struct vec_s
{
    void *data;
    size_t elem_size, capacity, size;
} vec_t;

vec_t vec_create(size_t capacity, size_t elem_size)
{
    vec_t vec;

    vec.elem_size = elem_size;
    vec.capacity = capacity;
    vec.size = 0;
    vec.data = (void *)malloc(elem_size * capacity);
    return vec;
}

void vec_free(vec_t *vec)
{
    free(vec->data);
}

void vec_push(vec_t *vec, void *elem)
{
    if (vec->size == vec->capacity)
    {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, vec->elem_size * vec->capacity);
    }

    memcpy((char *)vec->data + vec->size*vec->elem_size, elem, vec->elem_size);
    vec->size++;
}

