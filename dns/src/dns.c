#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <getopt.h>
#include "iface.h"

static const char *_SOCKET_DNS_SERVER = "223.5.5.5";

int main(int argc, char *argv[]) {
    const char* server = _SOCKET_DNS_SERVER;
    const char* iface = NULL;
    const char* type = "A";

    int opt;
    while ((opt = getopt(argc, argv, "I:d:t:")) != -1) {
        switch (opt) {
            case 'I': iface = optarg; break;
            case 'd': server = optarg; break;
            case 't': type = optarg; break;
            default:
                fprintf(stderr, "Usage: %s -I <interface> [-d <server>] [-t <type>] <domain>\n", argv[0]);
                return 1;
        }
    }

    if (optind >= argc || iface == NULL) {
        fprintf(stderr, "Usage: %s -I <interface> [-d <server>] [-t <type>] <domain>\n", argv[0]);
        return 1;
    }

    const char* domain = argv[optind];
    struct dnsaddr* addrs = NULL;
    int count = 0;
    dns_iface(domain, server, iface, type, &addrs, &count);
    for (int i = 0; i < count; i++) {
        struct dnsaddr* addr = addrs + i;
        printf("[DNS Answer] Domain: %s, Type: %s, IP : %s, TTL: %d.\n", domain, type, inet_ntoa(addr->addr), addr->ttl);
    }
    if (addrs) free(addrs);
    addrs = NULL;
    return 1;
}
