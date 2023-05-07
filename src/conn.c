#include <pthread.h>

#include "conn.h" 
#include "macro.h"

static const pthread_mutex_t s_pool_lock = PTHREAD_MUTEX_INITIALIZER; 

struct ConnectionPool {
    int pool_size; /* Capped at FD_SETSIZE */
    struct {
        fd_set rd_set;
        fd_set wr_set;
    } rdwr_fd_sets; 
};

ConnectionPool *conn_pool_init() {
    UNUSED(s_pool_lock);
    return NULL;
}

int conn_insert_fd(ConnectionPool *conn_pool, int fd) {
    UNUSED(conn_pool);
    UNUSED(fd);
    abort();
}
int conn_remove_fd(ConnectionPool *conn_pool, int fd) {
    UNUSED(conn_pool);
    UNUSED(fd);
    abort();
}
void conn_destroy(ConnectionPool *conn_pool) {
    UNUSED(conn_pool);
    abort();
}
int conn_copy_fd_sets(ConnectionPool *conn_pool, 
        fd_set *rd_set_cpy, fd_set *wr_set_cpy) {
    UNUSED(conn_pool);
    UNUSED(rd_set_cpy);
    UNUSED(wr_set_cpy);
    abort();
}
int conn_get_pool_size(ConnectionPool *conn_pool) {
    UNUSED(conn_pool);
    abort();
}
