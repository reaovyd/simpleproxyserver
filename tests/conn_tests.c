#include <criterion/criterion.h>

#include "conn.h"

#define CONNPOOL_NOTNULL(conn_pool) \
    do { \
        cr_assert_not_null(conn_pool, "Expected a non-null value from conn_pool. Memory allocation may have potentially failed.");\
    } while(0); \
        
Test(conn_suite, conn_init_1) {
    ConnectionPool *conn_pool = conn_pool_init();
    CONNPOOL_NOTNULL(conn_pool);
}

Test(conn_suite, conn_insert_connection_1) {
    ConnectionPool *conn_pool = conn_pool_init();
    CONNPOOL_NOTNULL(conn_pool);

    int insert_status = conn_insert_fd(conn_pool, 4); /* some fake fd */ 
    cr_assert_eq(insert_status, 0, "Expected insert to succeed, but got status %d", insert_status);
}

Test(conn_suite, conn_insert_connection_2) {
    ConnectionPool *conn_pool = conn_pool_init();
    CONNPOOL_NOTNULL(conn_pool);

    for(int i = 0; i < 1024; ++i) {
        conn_insert_fd(conn_pool, 4); 
    }
    int insert_status = conn_insert_fd(conn_pool, 4); 
    cr_assert_eq(insert_status, -1, "Expected insert to fail, but got status %d", insert_status);
}

Test(conn_suite, conn_insert_connection_3) {
    int insert_status = conn_insert_fd(NULL, 4); 
    cr_assert_eq(insert_status, -1, "Expected insert to fail, but got status %d", insert_status);
}

Test(conn_suite, conn_insert_connection_correct_size_1) {
    ConnectionPool *conn_pool = conn_pool_init();
    CONNPOOL_NOTNULL(conn_pool);
    int insert_status;

    for(int i = 0; i < 512; ++i) {
        insert_status = conn_insert_fd(conn_pool, i);
        cr_assert_eq(insert_status, 0, "Expected insert to succeed, but got status %d", insert_status);
    }
    cr_assert_eq(conn_get_pool_size(conn_pool), 512, "Expected insert to fail, but got status %d", insert_status);
}

Test(conn_suite, conn_get_fd_set_1) {
    ConnectionPool *conn_pool = conn_pool_init();
    CONNPOOL_NOTNULL(conn_pool);
    cr_assert_eq(conn_copy_fd_sets(NULL, NULL, NULL, NULL), -1, "Expected copy_fd to fail, but got a succeeding status"); 
}

Test(conn_suite, conn_get_fd_set_2) {
    ConnectionPool *conn_pool = conn_pool_init();
    CONNPOOL_NOTNULL(conn_pool);

    conn_insert_fd(conn_pool, 0);
    conn_insert_fd(conn_pool, 3);
    conn_insert_fd(conn_pool, 7);
    conn_insert_fd(conn_pool, 11);

    fd_set rd_cpy, wr_cpy;
    int nfds;

    int ccfs_status = conn_copy_fd_sets(conn_pool, &rd_cpy, &wr_cpy, &nfds);
    cr_assert_eq(ccfs_status, 0, "Expected copy_fd to succeed, but got a failing status %d", ccfs_status); 
    cr_assert(FD_ISSET(0, &rd_cpy) != 0, "rd_cpy does not contain the fd 0");
    cr_assert(FD_ISSET(3, &rd_cpy) != 0, "rd_cpy does not contain the fd 0");
    cr_assert(FD_ISSET(7, &rd_cpy) != 0, "rd_cpy does not contain the fd 0");
    cr_assert(FD_ISSET(11, &rd_cpy) != 0, "rd_cpy does not contain the fd 0");
    cr_assert(FD_ISSET(0, &wr_cpy) != 0, "wr_cpy does not contain the fd 0");
    cr_assert(FD_ISSET(3, &wr_cpy) != 0, "wr_cpy does not contain the fd 0");
    cr_assert(FD_ISSET(7, &wr_cpy) != 0, "wr_cpy does not contain the fd 0");
    cr_assert(FD_ISSET(11, &wr_cpy) != 0, "wr_cpy does not contain the fd 0");

    cr_assert_eq(nfds, 12, "nfds was not the maximum value that was expected; Got %d but expected 12", nfds);
}

Test(conn_suite, conn_get_pool_size_1) {
    ConnectionPool *conn_pool = conn_pool_init();
    CONNPOOL_NOTNULL(conn_pool);

    int pool_size;

    conn_insert_fd(conn_pool, 4); 
    conn_insert_fd(conn_pool, 4);
    conn_insert_fd(conn_pool, 4);
    conn_insert_fd(conn_pool, 4);

    pool_size = conn_get_pool_size(conn_pool);
    cr_assert_eq(pool_size, 1, "Expected only a size of 1, but got %d", pool_size);
}

Test(conn_suite, conn_get_pool_size_2) {
    int pool_size;

    pool_size = conn_get_pool_size(NULL);
    cr_assert_eq(pool_size, -1, "Expected call to fail, but got %d", pool_size);
}

Test(conn_suite, conn_remove_connection_1) {
    ConnectionPool *conn_pool = conn_pool_init();
    CONNPOOL_NOTNULL(conn_pool);

    int remove_status = conn_remove_fd(conn_pool, 90);

    cr_assert_eq(remove_status, -1, "Expected call to fail, but got %d", remove_status);
}

Test(conn_suite, conn_remove_connection_2) {
    ConnectionPool *conn_pool = conn_pool_init();
    CONNPOOL_NOTNULL(conn_pool);

    conn_insert_fd(conn_pool, 90);
    int remove_status = conn_remove_fd(conn_pool, 90);

    cr_assert_eq(remove_status, 0, "Expected call to succeed, but got %d", remove_status);
}

Test(conn_suite, conn_remove_connection_3) {
    ConnectionPool *conn_pool = conn_pool_init();
    CONNPOOL_NOTNULL(conn_pool);

    int remove_status = conn_remove_fd(NULL, 90);

    cr_assert_eq(remove_status, -1, "Expected call to fail, but got %d", remove_status);
}
