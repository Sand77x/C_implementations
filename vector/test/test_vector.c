#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "unity.h"
#include "unity_internals.h"
#include "vector.h"

#define SKIP_IF_SETUP_ERROR(void) \
    if (!setup_successful)        \
        TEST_IGNORE_MESSAGE("Skipping test due to setup error.");

Vector* v;
Vector* empty;
int res, output;

int setup_successful = 1;

void segfault_handler(int signal)
{
    TEST_FAIL_MESSAGE("[ERROR] SEGFAULT CAUGHT.");
}

void setUp(void)
{
    signal(SIGSEGV, segfault_handler);
    int arr[] = { 1, 2, 3, 4, 5 };
    v = VECTOR_create_from_array(arr, 5);
    empty = VECTOR_create();

    if (v == NULL) {
        setup_successful = 0;
    }

    if (empty == NULL) {
        setup_successful = 0;
    }
}

void tearDown(void)
{
    signal(SIGSEGV, segfault_handler);
    VECTOR_destroy(v);
    VECTOR_destroy(empty);
}

void test_VECTOR_create(void)
{
    SKIP_IF_SETUP_ERROR();
    Vector* new_vec = VECTOR_create();
    if (new_vec == NULL) {
        TEST_FAIL_MESSAGE("[ERROR] Vector creation failed when it should have succeeded.");
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, new_vec->size, "[ERROR] v->size should be 0.");
    TEST_ASSERT_EQUAL_MESSAGE(0, new_vec->capacity, "[ERROR] v->capacity should be 0.");
    TEST_ASSERT_EQUAL_MESSAGE(NULL, new_vec->vector, "[ERROR] v->vector should be NULL");
    free(new_vec);
}

void test_VECTOR_create_from_array(void)
{
    SKIP_IF_SETUP_ERROR();
    int arr[] = { 5, 4, 3, 2, 1 };
    Vector* new_vec = VECTOR_create_from_array(arr, 5);
    if (new_vec == NULL) {
        TEST_FAIL_MESSAGE("[ERROR] Vector failed to create.");
    }
    TEST_ASSERT_EQUAL_MESSAGE(5, new_vec->size, "[ERROR] v->size should be 5.");
    TEST_ASSERT_EQUAL_MESSAGE(5, new_vec->capacity, "[ERROR] v->capacity should be 0.");
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(arr, new_vec->vector, 5, "[ERROR] v->vector should contain { 5, 4, 3, 2, 1 }.");
    free(new_vec);
    free(new_vec->vector);
}

void test_VECTOR_destroy(void)
{
    SKIP_IF_SETUP_ERROR();
    VECTOR_destroy(v);
    TEST_PASS();
}

void test_VECTOR_push()
{
    SKIP_IF_SETUP_ERROR();
    int arr[] = { 1, 2, 3, 4, 5, 99 };
    int arr2[] = { 1, 2, 3, 4, 5, 99, 100 };
    int arr3[] = { 99 };
    // resize
    VECTOR_push(v, 99, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_SUCCESS, res, "[ERROR] result should be VECTOR_SUCCESS.");
    TEST_ASSERT_EQUAL_MESSAGE(6, v->size, "[ERROR] v->size should be 6.");
    TEST_ASSERT_EQUAL_MESSAGE(10, v->capacity, "[ERROR] v->capacity should be 10.");
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(arr, v->vector, 6, "[ERROR] v->vector should contain { 1, 2, 3, 4, 5, 99 }.");
    // no resize
    VECTOR_push(v, 100, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_SUCCESS, res, "[ERROR] result should be VECTOR_SUCCESS.");
    TEST_ASSERT_EQUAL_MESSAGE(7, v->size, "[ERROR] v->size should be 7.");
    TEST_ASSERT_EQUAL_MESSAGE(10, v->capacity, "[ERROR] v->capacity should be 10.");
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(arr2, v->vector, 7, "[ERROR] v->vector should contain { 1, 2, 3, 4, 5, 99, 100 }.");
    // push when 0 cap
    VECTOR_push(empty, 99, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_SUCCESS, res, "[ERROR] result should be VECTOR_SUCCESS.");
    TEST_ASSERT_EQUAL_MESSAGE(1, empty->size, "[ERROR] v->size should be 1.");
    TEST_ASSERT_EQUAL_MESSAGE(1, empty->capacity, "[ERROR] v->capacity should be 1.");
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(arr3, empty->vector, 1, "[ERROR] v->vector should contain { 99 }.");
}

void test_VECTOR_pop()
{
    SKIP_IF_SETUP_ERROR();
    int arr[] = { 1, 2, 3, 4 };
    int arr2[] = { 1, 2 };
    // no resize
    output = VECTOR_pop(v, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_SUCCESS, res, "[ERROR] result should be VECTOR_SUCCESS.");
    TEST_ASSERT_EQUAL_MESSAGE(5, output, "[ERROR] output should be 5.");
    TEST_ASSERT_EQUAL_MESSAGE(4, v->size, "[ERROR] v->size should be 4.");
    TEST_ASSERT_EQUAL_MESSAGE(5, v->capacity, "[ERROR] v->capacity should be 5.");
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(arr, v->vector, 4, "[ERROR] v->vector should contain { 1, 2, 3, 4 }.");
    // resize
    VECTOR_pop(v, &res);
    output = VECTOR_pop(v, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_SUCCESS, res, "[ERROR] result should be VECTOR_SUCCESS.");
    TEST_ASSERT_EQUAL_MESSAGE(3, output, "[ERROR] output should be 3.");
    TEST_ASSERT_EQUAL_MESSAGE(2, v->size, "[ERROR] v->size should be 2.");
    TEST_ASSERT_EQUAL_MESSAGE(2, v->capacity, "[ERROR] v->capacity should be 2.");
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(arr2, v->vector, 2, "[ERROR] v->vector should contain { 1, 2 }.");
    // pop makes vector empty
    VECTOR_pop(v, &res);
    output = VECTOR_pop(v, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_SUCCESS, res, "[ERROR] result should be VECTOR_SUCCESS.");
    TEST_ASSERT_EQUAL_MESSAGE(1, output, "[ERROR] output should be 1.");
    TEST_ASSERT_EQUAL_MESSAGE(0, v->size, "[ERROR] v->size should be 0.");
    TEST_ASSERT_EQUAL_MESSAGE(0, v->capacity, "[ERROR] v->capacity should be 0.");
    // pop when empty
    VECTOR_pop(empty, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_ERROR_OOB, res, "[ERROR] res should be VECTOR_OUT_OF_BOUNDS.");
}

void test_VECTOR_front()
{
    SKIP_IF_SETUP_ERROR();
    output = VECTOR_front(v, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_SUCCESS, res, "[ERROR] result should be VECTOR_SUCCESS.");
    TEST_ASSERT_EQUAL_MESSAGE(1, output, "[ERROR] output should be 1.");
    // front when empty
    VECTOR_front(empty, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_ERROR_OOB, res, "[ERROR] res should be VECTOR_OUT_OF_BOUNDS.");
}

void test_VECTOR_back()
{
    SKIP_IF_SETUP_ERROR();
    output = VECTOR_back(v, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_SUCCESS, res, "[ERROR] result should be VECTOR_SUCCESS.");
    TEST_ASSERT_EQUAL_MESSAGE(5, output, "[ERROR] output should be 5.");
    // back when empty
    VECTOR_back(empty, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_ERROR_OOB, res, "[ERROR] res should be VECTOR_OUT_OF_BOUNDS.");
}

void test_VECTOR_at()
{
    SKIP_IF_SETUP_ERROR();
    output = VECTOR_at(v, 2, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_SUCCESS, res, "[ERROR] result should be VECTOR_SUCCESS.");
    TEST_ASSERT_EQUAL_MESSAGE(3, output, "[ERROR] output should be 3.");
    // at when empty
    VECTOR_at(empty, 0, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_ERROR_OOB, res, "[ERROR] res should be VECTOR_OUT_OF_BOUNDS.");
    // at when oob
    VECTOR_at(v, 5, &res);
    TEST_ASSERT_EQUAL_MESSAGE(VECTOR_ERROR_OOB, res, "[ERROR] res should be VECTOR_OUT_OF_BOUNDS.");
}

void test_VECTOR_size()
{
    SKIP_IF_SETUP_ERROR();
    output = VECTOR_size(v);
    TEST_ASSERT_EQUAL_MESSAGE(5, output, "[ERROR] output should be 5.");
    output = VECTOR_size(empty);
    TEST_ASSERT_EQUAL_MESSAGE(0, output, "[ERROR] output should be 0.");
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_VECTOR_create);
    RUN_TEST(test_VECTOR_create_from_array);
    RUN_TEST(test_VECTOR_destroy);
    RUN_TEST(test_VECTOR_push);
    RUN_TEST(test_VECTOR_pop);
    RUN_TEST(test_VECTOR_front);
    RUN_TEST(test_VECTOR_back);
    RUN_TEST(test_VECTOR_at);
    RUN_TEST(test_VECTOR_size);
    return UNITY_END();
}
