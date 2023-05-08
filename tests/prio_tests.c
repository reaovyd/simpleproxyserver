#include <criterion/criterion.h>

#include "prio.h"

#define PRIOQUEUE_NOTNULL(pq) \
    do { \
        cr_assert_not_null(pq, "Expected a non-null value from pq. Memory allocation may have potentially failed.");\
    } while(0); \

Test(conn_suite, prio_init_1) {
    PrioQueue *pq = prio_init();
    PRIOQUEUE_NOTNULL(pq);
}

Test(conn_suite, prio_peek_max_1) {
    PrioQueue *pq = prio_init();
    PRIOQUEUE_NOTNULL(pq);

    int val;

    int ppm_status = prio_peek_max(pq, &val);

    cr_assert_eq(ppm_status, -1, "Expected -1 but got %d", ppm_status);
}

Test(conn_suite, prio_peek_max_2) {
    PrioQueue *pq = prio_init();
    PRIOQUEUE_NOTNULL(pq);

    int val;
    prio_insert(pq, 33);
    prio_insert(pq, 333);
    prio_insert(pq, 3333);

    int ppm_status = prio_peek_max(pq, &val);

    cr_assert_eq(ppm_status, 0, "Expected 0 but got %d", ppm_status);
    cr_assert_eq(val, 3333, "Expected 3333 but got %d", val);
}

Test(conn_suite, prio_peek_max_3) {
    PrioQueue *pq = prio_init();
    PRIOQUEUE_NOTNULL(pq);

    prio_insert(pq, 33);
    prio_insert(pq, 333);
    prio_insert(pq, 3333);

    int ppm_status = prio_peek_max(pq, NULL);

    cr_assert_eq(ppm_status, -1, "Expected -1 but got %d", ppm_status);
}

Test(conn_suite, prio_insert_1) {
    PrioQueue *pq = prio_init();
    PRIOQUEUE_NOTNULL(pq);

    int status = prio_insert(pq, 33);
    status = prio_insert(pq, 333);
    status = prio_insert(pq, 3333);

    cr_assert_eq(status, 0, "Expected 0 but got %d", status);
}

Test(conn_suite, prio_insert_2) {
    PrioQueue *pq = prio_init();
    PRIOQUEUE_NOTNULL(pq);

    int status;

    for(int i = 0; i < 1024; ++i) {
        status = prio_insert(pq, i);
    }
    status = prio_insert(pq, 123);

    cr_assert_eq(status, -1, "Expected -1 but got %d", status);
}

Test(conn_suite, prio_remove_max_1) {
    PrioQueue *pq = prio_init();
    PRIOQUEUE_NOTNULL(pq);

    int val;
    int status = prio_remove_max(pq, &val);
    cr_assert_eq(status, -1, "Expected -1 but got %d", status);
}

Test(conn_suite, prio_remove_max_2) {
    PrioQueue *pq = prio_init();
    PRIOQUEUE_NOTNULL(pq);

    int status;
    int val;

    prio_insert(pq, 12321312);
    status = prio_remove_max(pq, &val);

    cr_assert_eq(status, 0, "Expected 0 but got %d", status);
    cr_assert_eq(val, 12321312, "Expected 12321312 but got %d", val);
}

Test(conn_suite, prio_remove_max_3) {
    PrioQueue *pq = prio_init();
    PRIOQUEUE_NOTNULL(pq);

    prio_insert(pq, 1231232);

    int status;
    status = prio_remove_max(pq, NULL);
    cr_assert_eq(status, -1, "Expected -1 but got %d", status);
}

Test(conn_suite, prio_remove_insert_1) {
    PrioQueue *pq = prio_init();
    PRIOQUEUE_NOTNULL(pq);

    int status;
    int val;

    prio_insert(pq, 12321312);
    prio_insert(pq, 122);
    prio_insert(pq, 124);
    prio_insert(pq, 1231);
    prio_insert(pq, 1029);
    prio_insert(pq, 54);
    prio_insert(pq, 131);
    prio_insert(pq, 133);
    prio_insert(pq, 132);
    prio_insert(pq, 1300);

    status = prio_remove_max(pq, &val);
    cr_assert_eq(val, 12321312, "Expected 12321312 but got %d", val);
    cr_assert_eq(status, 0, "Expected 0 but got %d", val);
    status = prio_remove_max(pq, &val);
    cr_assert_eq(status, 0, "Expected 0 but got %d", val);
    cr_assert_eq(val, 1300, "Expected 1300 but got %d", val);
    prio_insert(pq, 1300);
    prio_insert(pq, 12321312);
    prio_insert(pq, 123213129);
    status = prio_remove_max(pq, &val);
    cr_assert_eq(status, 0, "Expected 0 but got %d", val);
    cr_assert_eq(val, 123213129, "Expected 123213129 but got %d", val);
}
