#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>

#include "server.h"
#include "macro.h"

static pthread_t s_server_thread_id; 
static volatile sig_atomic_t s_server_running = PROXY_SERVER_RUNNING;
static int s_listenfd = -1;

static void terminate_listenfd_atomic() {
    sigset_t set, oldset;
    sigemptyset(&set);

    sigaddset(&set, SIGHUP);

    sigprocmask(SIG_BLOCK, &set, &oldset);
    if(s_listenfd != -1) {
        close(s_listenfd);
        s_listenfd = -1;
    }
    sigprocmask(SIG_SETMASK, &oldset, NULL);
}

static void terminate_handler(int signum) {
    if(s_server_thread_id != pthread_self()) {
        pthread_kill(s_server_thread_id, signum);
    } else {
        /* ensures that main_thread is the only one that handles terminate */
        s_server_running = PROXY_SERVER_TERMINATED;
        terminate_listenfd_atomic();
    }
}

static void set_signals() {
    struct sigaction act;
    memset(&act, 0x0, sizeof(act));
    act.sa_handler = terminate_handler;

    sigaction(SIGHUP, &act, NULL);
}

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

    s_listenfd = setup_listenfd(port);
    if(s_listenfd == -1) {
        return -1;
    }

    s_server_thread_id = pthread_self();
    set_signals();

    struct sockaddr_storage accept_addr;
    socklen_t addrlen;

    printf("Proxy server is now listening on port %s\n", port);

    while(s_server_running == PROXY_SERVER_RUNNING) {
        addrlen = sizeof(accept_addr);

        int connfd = accept(s_listenfd, (struct sockaddr *)&accept_addr, &addrlen);  
        if(connfd == -1) {
            s_server_running = PROXY_SERVER_TERMINATED;
            terminate_listenfd_atomic();
        }
        // possibly accept finished but the signal terminate happened here
        if(s_server_running == PROXY_SERVER_TERMINATED) {
            if(connfd != -1)
                close(connfd);
            break;
        }
        //char *stream_buf;
        //size_t sz;

        //FILE *fstream; 

        //fstream = open_memstream(&stream_buf, &sz);

        //char buf[512];
        //while(1) {
        //    ssize_t nread, nwrite;

        //    nread = recv(connfd, buf, 512, 0);
        //    //if(nread == 0) {
        //    //    close(connfd);
        //    //    break;
        //    //} else if(nread == -1) {
        //    //    break;
        //    //} else {
        //    //    char *buf_ptr;

        //    //    buf_ptr = buf;
        //    //    while(nread > 0) {
        //    //        nwrite = fwrite(buf_ptr, sizeof(char), 512, fstream);
        //    //        if(nwrite == -1) {
        //    //            if(errno == EINTR) {
        //    //                nwrite = 0;
        //    //            }
        //    //        }
        //    //        nread -= nwrite;
        //    //        buf_ptr += nwrite;
        //    //    }
        //    //}
        //}
        //fclose(fstream);
        //printf("%s", stream_buf);
        //fflush(stdout);
        //free(stream_buf);

        close(connfd);
    }

    return 0;
}
