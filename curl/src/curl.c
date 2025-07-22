#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <getopt.h>
#include <curl/curl.h>

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

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    fwrite(ptr, size, nmemb, (FILE*)userdata);
    fflush(stdout);
    return total_size;
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
    if (count > 0) {
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
    //combine url
    int port = 80;
    char url[128];
    sprintf(url, "http://%s:%d/", ip_str, port);
    printf("request url: %s\n", url);
    //curl request
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    /*ubuntu: sudo apt install --reinstall libcurl4-openssl-dev*/
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);

        /* curl also support bind interface */
        // curl_easy_setopt(curl, CURLOPT_INTERFACE, iface);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Failed to init curl\n");
    }
    curl_global_cleanup();
    return 0;
}
