#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <getopt.h>

struct dnsaddr {
    struct in_addr addr;
    uint32_t ttl;
};
extern int dns_iface(const char* domain, const char* server, const char* iface, const char* type, struct dnsaddr** addrs, int *count);
extern int socket_iface_opt(const char* device);
extern int socket_iface(int domain, int type, int protocol);
static const char *_SOCKET_DNS_SERVER = "223.5.5.5";

int socket(int domain, int type, int protocol) {
    return socket_iface(domain, type, protocol);
}

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
                return 0;
        }
    }

    if (optind >= argc || iface == NULL) {
        fprintf(stderr, "Usage: %s -I <interface> [-d <server>] [-t <type>] <domain>\n", argv[0]);
        return 0;
    }
    const char* domain = argv[optind];
    printf("iface:%s, domain:%s\n", iface, domain);

    struct dnsaddr* addrs = NULL;
    int count = 0;
    dns_iface(domain, server, iface, type, &addrs, &count);
    char ip_str[INET_ADDRSTRLEN];

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);

    if (count > 0) {
        addr.sin_addr.s_addr = addrs->addr.s_addr;
        inet_ntop(AF_INET, &(addrs->addr), ip_str, sizeof(ip_str));
        printf("Resolved IP: %s\n", ip_str);
    }
    free(addrs);
    addrs = NULL;
    if (count <= 0) {
        fprintf(stderr, "dns resove IP failed.\n");
        return 1;
    }
    socket_iface_opt(iface);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 0;
    }

    printf("Connecting to %s\n", ip_str);
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        return 0;
    }
    printf("Connected successfully!\n");
    close(sock);
    return 0;
}
