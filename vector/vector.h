/*
 * A resizable array that is dynamically allocated and can grow and shrink
 * based on the amount of items within it.
 * For this implementation, should support only int32. */

#define VECTOR_SUCCESS 0
#define VECTOR_ERROR_OOB 1
#define VECTOR_ERROR_ALLOC 2

typedef struct Vector {
    int* vector;
    int size;
    int capacity;
} Vector;

/*
 * Initialize an empty vector */
Vector* VECTOR_create(void);

/*
 * Initialize a populated vector. */
Vector* VECTOR_create_from_array(int* arr, int size);

/*
 * Destroy a vector. */
void VECTOR_destroy(Vector* v);

/*
 * Add an item to the end of vector */
void VECTOR_push(Vector* v, int new_item, int* success);

/*
 * Remove an item from the end of vector and return the item. */
int VECTOR_pop(Vector* v, int* success);

/*
 * Get the first element in the vector. */
int VECTOR_front(Vector* v, int* success);

/*
 * Get the last element in the vector. */
int VECTOR_back(Vector* v, int* success);

/*
 * Get the vector element in the specified index. */
int VECTOR_at(Vector* v, int idx, int* success);

/*
 * Get the current length of the vector. */
int VECTOR_size(Vector* v);
