#ifndef VND_DNS_IFACE_H
#define VND_DNS_IFACE_H
#include <net/if.h>
struct dnsaddr {
    struct in_addr addr;
    uint32_t ttl;
};
int dns_iface(const char* domain, const char* server, const char* iface, const char* type, struct dnsaddr** addrs, int *count);
#endif
