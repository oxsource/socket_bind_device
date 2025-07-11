#include <dlfcn.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <getopt.h>
#include <arpa/inet.h>
#include "socket_iface.h"

int socket(int domain, int type, int protocol) {
    return socket_iface(domain, type, protocol);
}

int main(int argc, char *argv[]) {
    const char* server = "223.5.5.5";
    const char* iface = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "I:d:t:")) != -1) {
        switch (opt) {
            case 'I': iface = optarg; break;
            default:
                fprintf(stderr, "Usage: %s -I <interface>\n", argv[0]);
                return 0;
        }
    }
    if (iface == NULL) {
        fprintf(stderr, "Usage: %s -I <interface>\n", argv[0]);
        return 0;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    if (inet_pton(AF_INET, server, &addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        return 0;
    }
    socket_iface_opt(iface);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 0;
    }
    printf("Connecting to %s\n", server);
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        return 0;
    }
    printf("Connected successfully!\n");
    close(sock);
    return 0;
}
