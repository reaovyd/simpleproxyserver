#include <pthread.h>
#include <stdio.h>

#include "conn.h" 
#include "macro.h"
#include "prio.h"

static const pthread_mutex_t s_pool_lock = PTHREAD_MUTEX_INITIALIZER; 

struct connectionPool {
    unsigned int pool_size; /* Capped at FD_SETSIZE */
    struct {
        fd_set rd_set;
        fd_set wr_set;
    } rdwr_fd_sets; 
    PrioQueue *pq;
};

ConnectionPool *conn_pool_init() {
    ConnectionPool *c_pool = malloc(sizeof(ConnectionPool));
    if(c_pool == NULL) {
        perror("malloc");
        return NULL;
    }
    UNUSED(s_pool_lock);

    c_pool->pool_size = 0;
    FD_ZERO(&c_pool->rdwr_fd_sets.rd_set);
    FD_ZERO(&c_pool->rdwr_fd_sets.wr_set);
    c_pool->pq = prio_init();
    if(c_pool->pq == NULL) {
        free(c_pool);
        perror("malloc");
        return NULL;
    }

    return c_pool;
}

int conn_insert_fd(ConnectionPool *conn_pool, int fd) {
    if(conn_pool == NULL) {
        return -1;
    }
    if(conn_pool->pool_size >= FD_SETSIZE) {
        return -1;
    }
    FD_SET(fd, &conn_pool->rdwr_fd_sets.rd_set); 
    FD_SET(fd, &conn_pool->rdwr_fd_sets.wr_set); 
    conn_pool->pool_size++;

    return 0;
}

int conn_remove_fd(ConnectionPool *conn_pool, int fd) {
    if(conn_pool == NULL) {
        return -1;
    }
    if(conn_pool->pool_size == 0) {
        return -1;
    }
    UNUSED(fd);
    abort();
}

void conn_destroy(ConnectionPool *conn_pool) {
    UNUSED(conn_pool);
    abort();
}

int conn_copy_fd_sets(ConnectionPool *conn_pool, 
        fd_set *rd_set_cpy, fd_set *wr_set_cpy, int *nfds) {
    UNUSED(conn_pool);
    UNUSED(rd_set_cpy);
    UNUSED(wr_set_cpy);
    UNUSED(nfds);

    abort();
}

int conn_get_pool_size(ConnectionPool *conn_pool) {
    UNUSED(conn_pool);
    abort();
}
