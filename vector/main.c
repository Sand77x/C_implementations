#include <stdio.h>

#include "vector.h"

int res;

void print_vector_info(Vector* v)
{
    printf("Size: %d, Cap: %d, Vector ptr: %p\n", v->size, v->capacity, v->vector);
}

int main()
{
    Vector* v = VECTOR_create();

    if (v == NULL) {
        printf("VECTOR ALLOCATION FAILED.\n");
        return 0;
    }

    printf("-----ADDING 128 ITEMS-----\n\n");
    print_vector_info(v);

    for (int i = 0; i < 128; i++) {
        int cap = v->capacity;
        printf("%d ", i);
        VECTOR_push(v, i, &res);

        if (res != 0) {
            printf("En error occured. %d\n", res);
            break;
        }

        if (v->capacity != cap) {
            printf("\nCAP: %d\n", v->capacity);
        }
    }

    printf("\n\n-----POPPING 128 ITEMS-----\n\n");
    print_vector_info(v);

    for (int i = 0; i < 128; i++) {
        int cap = v->capacity;
        printf("%d ", VECTOR_pop(v, &res));

        if (res != 0) {
            printf("En error occured. %d\n", res);
            break;
        }

        if (v->capacity != cap) {
            printf("\nCAP: %d\n", v->capacity);
        }
    }

    VECTOR_destroy(v);
    return 0;
}
