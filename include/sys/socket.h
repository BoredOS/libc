/* Minimal sys/socket.h for BoredOS userland */
#ifndef SYS_SOCKET_H
#define SYS_SOCKET_H

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

typedef uint16_t sa_family_t;
typedef uint32_t socklen_t;

#define AF_UNIX 1
#define AF_LOCAL AF_UNIX
#define AF_INET 2
#define AF_INET6 10

#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define SOCK_RAW 3

struct in_addr {
    uint32_t s_addr;
};

struct sockaddr_in {
    sa_family_t sin_family;
    uint16_t sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
};

#define INADDR_ANY 0

#define htons(x) ((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))
#define htonl(x) ((((x) & 0x000000FF) << 24) | \
                  (((x) & 0x0000FF00) << 8)  | \
                  (((x) & 0x00FF0000) >> 8)  | \
                  (((x) & 0xFF000000) >> 24))
#define ntohs(x) htons(x)
#define ntohl(x) htonl(x)

struct sockaddr {
    sa_family_t sa_family;
    char sa_data[14];
};

#define UNIX_PATH_MAX 108
struct sockaddr_un {
    sa_family_t sun_family;
    char sun_path[UNIX_PATH_MAX];
};

/* Basic socket prototypes (implemented by libc/syscall glue or lwIP)
 * ssize_t/read/write/close are in other headers.
 */
int socket(int domain, int type, int protocol);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
int shutdown(int sockfd, int how);

#ifndef NET_IPV4_ADDRESS_T_DEFINED
#define NET_IPV4_ADDRESS_T_DEFINED
typedef struct { uint8_t bytes[4]; } net_ipv4_address_t;
#endif
int dns_lookup(const char *name, net_ipv4_address_t *out_ip);
int icmp_ping(const net_ipv4_address_t *dest_ip);

#endif
