#include <dlfcn.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

static int (*real_socket)(int, int, int) = NULL;

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static int bind_device(int sockfd, int domain, int type) {
    int value = 1;
    const char* device = getenv("SOCKET_DEVICE_NAME");
    if (!device) return value;
    printf("setsockopt bind_device iface: %s\n", device);
    if (!(domain == AF_INET || domain == AF_INET6)) return value;
    if (!(type == SOCK_STREAM || type == SOCK_DGRAM)) return value;
    pthread_mutex_lock(&lock);
    if (sockfd < 0 || setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, device, strlen(device) + 1)) {
        perror("setsockopt bind_device failed\b");
        value = -1;
    }
    pthread_mutex_unlock(&lock);
    return value;
}

//https://man7.org/linux/man-pages/man7/socket.7.html
int socket(int domain, int type, int protocol) {
    if (!real_socket) {
        real_socket = dlsym(RTLD_NEXT, "socket");
    }
    int fd = real_socket(domain, type, protocol);
    if (fd < 0 || bind_device(fd, domain, type)) return fd;
    close(fd);
    return -1;
}
