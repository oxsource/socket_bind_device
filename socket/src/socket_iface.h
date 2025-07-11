#ifndef VND_SOCKET_IFACE_H
#define VND_SOCKET_IFACE_H
int socket_iface_opt(const char* device);
int socket_iface(int domain, int type, int protocol);
#endif
