/**
 * @file conn.h
 * @brief This interface is the specification
 * for the `ConnectionPool` object which handles
 * all of the current connections that the
 * proxy server is handling. It is important
 * to note that each function occurs atomically
 * in terms of multiple threads calling the 
 * functions. This is done through a simple mutex
 * that locks before the function runs and 
 * after the function runs so that data is consistent
 * and synchronized across each thread.
 *
 */

#ifndef CONN_H
#define CONN_H

#include <sys/select.h>

/* TODO Swapping to a more scalable version */

/**
 * @struct ConnectionPool conn.h include/conn.h
 * @brief This is the object that represents
 * the connection pool. This is the object
 * responsible for holding each of the user's
 * connection that can later be passed to an
 * event handler like `select(2)` and, then,
 * further processes the connections that correspond
 * to some socket file descriptor. The structure looks
 * like this in the source file:
 *
 * ```
 * struct connectionPool {
 *     unsigned int pool_size; // capped at FD_SETSIZE
 *     struct {
 *         fd_set rd_set;
 *         fd_set wr_set;
 *     } rdwr_fd_sets; 
 *     PrioQueue *pq;
 * };
 *
 * ```
 *
 */

typedef struct connectionPool ConnectionPool;

/**
 * @brief Initializes a new connection pool to handle
 * user connections. The caller should be using
 * a function like `select(2)` to handle the pool.
 * The initialiation fails if memory allocation
 * for the ConnectionPool fails.
 *
 * @return On success, it returns a pointer to the
 * new connection pool. Otherwise, on failure, it
 * returns NULL.
 *
 */
extern ConnectionPool *conn_pool_init(void);

/** 
 * @brief Inserts a new connection into the connection
 * pool. The new connection is described by the
 * file descriptor fd and the connection pool
 * is represented as conn_pool. The function fails
 * if including this new connection exceeds the maximum 
 * number of possible connections that select(2) 
 * can handle. In this case, it is FD_SETSIZE 
 * (which is defined to be 1024 as specified in select(2)) 
 * connections. The function also fails if the conn_pool
 * pointer points to a `NULL` address. If the connection
 * pool holds the current fd already, nothing is done,
 * but the call succeeds.
 *
 * @param conn_pool A pointer to a connection pool 
 * that holds all of the current connections
 * @param fd The socket file descriptor that represents
 * the new connection
 * @return 0 if the new connection has been succesfully
 * inserted into the pool. Otherwise, it returns -1.
 *
 */

extern int conn_insert_fd(ConnectionPool *conn_pool, int fd); 

/**
 * @brief Removes a connection from the connection pool. 
 * The connection that gets removed is specified by 
 * the file descriptor fd. In addition, it also fails 
 * if conn_pool is `NULL`. If fd is not in the pool, then
 * the call still succeeds. The call, however, does fail
 * if the pool size is currently less than or equal to 0.
 *
 * @param conn_pool A pointer to a connection pool 
 * that holds all of the current connections
 * @param fd The socket file descriptor that represents
 * the connection to remove
 * @return 0 if the new connection has been succesfully
 * removed from the pool. Otherwise, it returns -1.
 *
 */

extern int conn_remove_fd(ConnectionPool *conn_pool, int fd);

/**
 * TODO honestly fix this and make it mroe clear 
 *
 * @brief This function shuts down any of the connections
 * that are still in the pool. The function invokes
 * the shutdown(2) to shut down any socket file 
 * descriptors that belong to that connection. In 
 * addition, calling this function will free the 
 * block being pointed to by conn_pool.
 *
 * @param conn_pool A pointer to a connection pool 
 * that holds all of the current connections
 *
 */

extern void conn_destroy(ConnectionPool *conn_pool);

/**
 * @brief This function takes as argument a pointer 
 * to a connection pool. The caller specifies the 
 * argument rd_set_cpy (which is the read set that 
 * will be passed to select(2)) and wr_set_cpy 
 * (which is the write set that will be passed to 
 * select), and the connection pool's fd sets are 
 * copied over to the respective copy set. The function 
 * fails if either of the pointers to the sets point 
 * to a `NULL` reference or if the connection pool 
 * points to a `NULL` value. This function also takes in a `nfds`
 * argument which is a pointer to an integer value. The max 
 * file descriptor value is copied over to that pointer.
 * If `nfds` points to a `NULL` value, then the function
 * call fails.
 *
 * @param conn_pool A pointer to a connection pool 
 * that holds all of the current connections
 * @param rd_set_cpy The read set passed by the caller
 * in which the function copies the pool's read set
 * to rd_set_cpy
 * @param wr_set_cpy The write set passed by the caller
 * in which the function copies the pool's write set
 * to wr_set_cpy 
 * @param nfds The pointer to an integer that will 
 * have its value to that address replaced by the 
 * current max file descriptor value
 * @return 0 if the copying succeeds. Otherwise, -1.
 *
 */

extern int conn_copy_fd_sets(ConnectionPool *conn_pool, 
        fd_set *rd_set_cpy, fd_set *wr_set_cpy, int *nfds);

/*
 * @brief This function takes as argument a pointer to a 
 * connection pool, specified by conn_pool. On success,
 * it returns the number of connections held by the 
 * connection pool. The call fails if conn_pool points
 * to a `NULL` pointer.
 *
 * @param  conn_pool The connection pool that holds all
 * of the current connections
 * @return On success, it returns the current number of
 * connections held by the pool. Otherwise, it returns
 * -1.
 *
 */
extern int conn_get_pool_size(ConnectionPool *conn_pool);

#endif /* CONN_H */
