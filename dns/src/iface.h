#ifndef VND_DNS_IFACE_H
#define VND_DNS_IFACE_H
#include <net/if.h>
int dns_iface(const char* domain, const char* server, const char* iface, const char* type, struct in_addr** addrs, int *count);
#endif
