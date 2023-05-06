#include <stdio.h>
#include <string.h>

#include "macro.h"
#include "server.h"

int main(int argc, char *argv[]) {
    if(argc != 3) {
        P_USAGE_EXIT(argv[0]);
    }
    if(strcmp(argv[1], "-p") != 0) {
        P_USAGE_EXIT(argv[0]);
    }

    if(run_proxy_server(argv[2]) == -1) {
        exit(EXIT_FAILURE);
    }

    return 0;
}
