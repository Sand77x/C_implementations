#include <stdlib.h>

#include "vector.h"

/*
 * This library implements a "Vector" or dynamic auto-resizing array.
 * Passing invalid vector pointers to any of these functions will cause undefined behavior.
 * This library does not use perror or sets errno.
 * Use the success variables to gauge success of Vector functions.
 * The creation functions return NULL if memory for a Vector was not properly allocated. */

Vector* VECTOR_create()
{
    Vector* v = malloc(sizeof(Vector));

    if (v == NULL) {
        return NULL;
    }

    v->vector = NULL;
    v->size = 0;
    v->capacity = 0;

    return v;
}

Vector* VECTOR_create_from_array(int* arr, int size)
{
    Vector* v = malloc(sizeof(Vector));

    if (v == NULL) {
        return NULL;
    }

    v->vector = malloc(sizeof(int) * size);
    v->capacity = size;
    v->size = size;

    if (v->vector == NULL) {
        free(v);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        *(v->vector + i) = *(arr + i);
    }

    return v;
}

void VECTOR_destroy(Vector* v)
{
    if (v == NULL) {
        return;
    }

    if (v->vector == NULL) {
        free(v->vector);
    }

    free(v);
}

void VECTOR_push(Vector* v, int new_item, int* success)
{
    if (v->size >= v->capacity) {
        if (v->capacity == 0)
            v->capacity = 1;
        else
            v->capacity *= 2;
        v->vector = realloc(v->vector, sizeof(int) * v->capacity);
    }

    if (v->vector == NULL) {
        *success = VECTOR_ERROR_ALLOC;
        return;
    }

    v->size++;
    v->vector[v->size - 1] = new_item;

    *success = VECTOR_SUCCESS;
}

int VECTOR_pop(Vector* v, int* success)
{
    if (v->capacity == 0) {
        *success = VECTOR_ERROR_OOB;
        return 0;
    }

    int back = v->vector[v->size - 1];
    int half_cap = v->capacity / 2;
    if (v->size - 1 <= half_cap) {
        v->vector = realloc(v->vector, sizeof(int) * half_cap);
        v->capacity = half_cap;
    }

    if (half_cap > 0 && v->vector == NULL) {
        *success = VECTOR_ERROR_ALLOC;
        return 0;
    }

    *success = VECTOR_SUCCESS;
    v->size--;
    return back;
}

int VECTOR_front(Vector* v, int* success)
{
    if (v->size == 0) {
        *success = VECTOR_ERROR_OOB;
        return 0;
    }

    *success = VECTOR_SUCCESS;
    return v->vector[0];
}

int VECTOR_back(Vector* v, int* success)
{
    if (v->size == 0) {
        *success = VECTOR_ERROR_OOB;
        return 0;
    }

    *success = VECTOR_SUCCESS;
    return v->vector[v->size - 1];
}

int VECTOR_at(Vector* v, int idx, int* success)
{
    if (v->size == 0 || idx < 0 || idx > v->size - 1) {
        *success = VECTOR_ERROR_OOB;
        return 0;
    }

    *success = VECTOR_SUCCESS;
    return v->vector[idx];
}

int VECTOR_size(Vector* v)
{
    return v->size;
}
