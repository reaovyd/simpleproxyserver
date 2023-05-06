#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include "server.h"
#include "macro.h"

static int32_t parse_port(char *port) {
    if(port == NULL) {
        return -1;
    }
    int32_t ret = 0;
    while (*port) {
        if(!isdigit(*port)) {
            return -1;
        }
        ret *= 10;
        ret += (*port - '0');
        port++;
    }

    return ret;
}

static int setup_listenfd(char *port) {
    struct protoent *pe_p;

    pe_p = getprotobyname("tcp");
    if(pe_p == NULL) {
        fprintf(stderr, "Could not find the TCP protocol. Ensure it is mapped in the /etc/protocols file.");
        return -1;
    }

    struct addrinfo hints, *res; /* res must be freed */
    int gai_status;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_flags      = AI_PASSIVE; 
    hints.ai_protocol   = pe_p->p_proto;

    if((gai_status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(gai_status));
        return -1;
    }

    int listenfd;

    listenfd = -1;

    for(struct addrinfo *cur_ai = res; cur_ai; cur_ai++) {
        if((listenfd = socket(cur_ai->ai_family, 
                        cur_ai->ai_socktype, cur_ai->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        int yes = 1;
        if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            close(listenfd);
            listenfd = -1;
            break;
        }

        if(bind(listenfd, cur_ai->ai_addr, cur_ai->ai_addrlen) == -1) {
            perror("bind");
            close(listenfd);
            listenfd = -1;
            break;
        }

        if(listen(listenfd, MAX_BACKLOG_SZ) == -1) {
            perror("listen");
            close(listenfd);
            listenfd = -1;
            break;
        }

        if(listenfd != -1) {
            /* Since we only need one binded one */
            break;
        }
    }

    freeaddrinfo(res);
    return listenfd;
}

int run_proxy_server(char *port) {
    int32_t port_val;
    if((port_val = parse_port(port)) == -1) {
        fprintf(stderr, "port value %s could not be parsed!\n", port);
        return -1;
    }

    int listenfd = setup_listenfd(port);
    if(listenfd == -1) {
        return -1;
    }

    struct sockaddr_storage accept_addr;
    socklen_t addrlen;

    printf("Proxy server is now listening on port %s\n", port);

    while(PROXY_SERVER_RUNNING) {
        addrlen = sizeof(accept_addr);
        int connfd = accept(listenfd, (struct sockaddr *)&accept_addr, &addrlen);  
        if(connfd == -1) {
            perror("accept");
            close(listenfd);
            return -1;
        }

        close(connfd);
    }

    return 0;
}
