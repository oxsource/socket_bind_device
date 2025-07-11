#include <dlfcn.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "socket_iface.h"

static int (*real_socket)(int, int, int) = NULL;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static const char* SOCKET_DEVICE = "SOCKET_DEVICE_NAME";

//https://man7.org/linux/man-pages/man7/socket.7.html
static int bind_to_device(int sockfd, int domain, int type) {
    int value = 1;
    if (!(domain == AF_INET || domain == AF_INET6)) return value;
    if (!(type == SOCK_STREAM || type == SOCK_DGRAM)) return value;
    const char* device = getenv(SOCKET_DEVICE);
    if (!device) return value;
    printf("setsockopt iface: %s\n", device);
    pthread_mutex_lock(&lock);
    if (sockfd < 0 || setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, device, strlen(device) + 1)) {
        perror("setsockopt failed\b");
        value = -1;
    }
    pthread_mutex_unlock(&lock);
    return value;
}

int socket_iface_opt(const char* device) {
    setenv(SOCKET_DEVICE, device, 1);
    return 1;
}

int socket_iface(int domain, int type, int protocol) {
    if (!real_socket) {
        real_socket = dlsym(RTLD_NEXT, "socket");
    }
    int fd = real_socket(domain, type, protocol);
    if (fd < 0 || bind_to_device(fd, domain, type)) return fd;
    close(fd);
    return -1;
}
