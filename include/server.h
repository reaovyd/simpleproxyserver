/**
 * @file server.h
 * @brief This has a single function 
 * where it begins to run the proxy 
 * server.
 *
 */

#ifndef SERVER_H
#define SERVER_H

/**
 * @brief This starts up a server on 
 * the calling thread. It takes as argument a 
 * port which must be an integer value. The 
 * server will create a new TCP socket that binds 
 * to the port specified by the argument *port*. 
 * Then, the server will be set up to listen for
 * any new TCP connections and accept such
 * connections. A client can send any HTTP
 * request and the server will handle the request 
 * by directing the request to the actual server 
 * the user wants to access. The function fails 
 * when any of the socket system calls, memory 
 * allocation calls, or any other system calls
 * fail. It also fails if the port is not a
 * short integer when parsed by the function. 
 *
 * @param port The port the server has its listening socket 
 *             bind to.
 * @return 0 if server successfully runs and terminates. 
 * Otherwise, it returns -1.
 *
 */
int run_proxy_server(char *port);

#endif /* SERVER_H */
