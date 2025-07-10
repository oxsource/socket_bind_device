#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/time.h>
#include "iface.h"

struct DNS_HEADER { 
    unsigned short id;
    unsigned char rd :1;
    unsigned char tc :1;
    unsigned char aa :1;
    unsigned char opcode :4;
    unsigned char qr :1;
    unsigned char rcode :4;
    unsigned char cd :1;
    unsigned char ad :1;
    unsigned char z :1;
    unsigned char ra :1;
    unsigned short q_count;
    unsigned short ans_count;
    unsigned short auth_count;
    unsigned short add_count;
};

struct QUESTION {
    unsigned short qtype;
    unsigned short qclass;
};

#pragma pack(push, 1)
struct R_DATA {
    unsigned short type;
    unsigned short _class;
    unsigned int ttl;
    unsigned short data_len;
};
#pragma pack(pop)

static unsigned char* ReadName(unsigned char* reader, unsigned char* buffer, int* count) {
    unsigned char* name = (unsigned char*) malloc(256);
    int jumped = 0, offset, i = 0;
    *count = 1;
    name[0] = '\0';

    while (*reader != 0) {
        if (*reader >= 192) {
            offset = (*reader) * 256 + *(reader + 1) - 49152;
            reader = buffer + offset - 1;
            jumped = 1;
        } else {
            name[i++] = *reader;
        }
        reader++;
        if (!jumped) (*count)++;
    }
    name[i] = '\0';
    if (jumped) (*count)++;
    return name;
}

static void ChangetoDnsNameFormat(unsigned char* dns, const char* host) {
    int lock = 0;
    char hostname[256];
    snprintf(hostname, sizeof(hostname), "%s.", host);

    for (int i = 0; i < strlen(hostname); i++) {
        if (hostname[i] == '.') {
            *dns++ = i - lock;
            for (; lock < i; lock++) {
                *dns++ = hostname[lock];
            }
            lock++;
        }
    }
    *dns++ = '\0';
}

static unsigned short getQueryType(const char* type) {
    if (strcasecmp(type, "A") == 0) return 1;
    if (strcasecmp(type, "AAAA") == 0) return 28;
    if (strcasecmp(type, "CNAME") == 0) return 5;
    if (strcasecmp(type, "MX") == 0) return 15;
    fprintf(stderr, "Unsupported query type: %s, defaulting to A\n", type);
    return 1;
}

int dns_iface(const char* domain, const char* server, const char* iface, const char* type, struct in_addr** addrs, int *count) {
    unsigned char buffer[65536], *qname, *reader;
    struct sockaddr_in dest;
    struct timeval start, end;
    unsigned short qtype = getQueryType(type);
    *count = 0;
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket creation failed");
        return -1;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, iface, strlen(iface) + 1) < 0) {
        perror("setsockopt SO_BINDTODEVICE failed");
        close(sockfd);
        return -2;
    }

    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt SO_RCVTIMEO failed");
        close(sockfd);
        return -3;
    }

    dest.sin_family = AF_INET;
    dest.sin_port = htons(53);
    dest.sin_addr.s_addr = inet_addr(server);

    struct DNS_HEADER *dns = (struct DNS_HEADER *) buffer;
    dns->id = htons(getpid());
    dns->qr = 0;
    dns->opcode = 0;
    dns->aa = 0;
    dns->tc = 0;
    dns->rd = 1;
    dns->ra = 0;
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1);
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;

    qname = &buffer[sizeof(struct DNS_HEADER)];
    ChangetoDnsNameFormat(qname, domain);

    struct QUESTION *qinfo = (struct QUESTION *) &buffer[sizeof(struct DNS_HEADER) + strlen((const char*) qname) + 1];
    qinfo->qtype = htons(qtype);
    qinfo->qclass = htons(1);

    int packet_size = sizeof(struct DNS_HEADER) + strlen((const char*) qname) + 1 + sizeof(struct QUESTION);

    gettimeofday(&start, NULL);
    if (sendto(sockfd, buffer, packet_size, 0, (struct sockaddr*)&dest, sizeof(dest)) < 0) {
        perror("sendto failed");
        close(sockfd);
        return -4;
    }

    socklen_t len = sizeof(dest);
    if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&dest, &len) < 0) {
        perror("recvfrom failed");
        close(sockfd);
        return -5;
    }
    gettimeofday(&end, NULL);

    long elapsed_ms = (end.tv_sec - start.tv_sec) * 1000 +
                      (end.tv_usec - start.tv_usec) / 1000;
    printf("DNS query time: %ld ms\n", elapsed_ms);

    dns = (struct DNS_HEADER*) buffer;
    reader = &buffer[sizeof(struct DNS_HEADER)];

    for (int i = 0; i < ntohs(dns->q_count); i++) {
        while (*reader != 0) reader++;
        reader++;
        reader += sizeof(struct QUESTION);
    }

    *addrs = malloc(sizeof(struct in_addr) * ntohs(dns->ans_count));
    if (!addrs) {
        perror("malloc addrs failed.");
        close(sockfd);
        return -6;
    }
    int stop;
    for (int i = 0; i < ntohs(dns->ans_count); i++) {
        unsigned char *name = ReadName(reader, buffer, &stop);
        reader += stop;

        struct R_DATA *resource = (struct R_DATA*)(reader);
        reader += sizeof(struct R_DATA);

        if (ntohs(resource->type) == 1) {
            memcpy(&((*addrs)[(*count)++].s_addr), reader, sizeof(in_addr_t));
        }
        reader += ntohs(resource->data_len);
        free(name);
    }

    close(sockfd);
    return 1;
}
