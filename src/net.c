#include "net.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int init_udp_socket()
{
    int sock_fd;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        return NO_SOCKET;
    }
    return sock_fd;
}

int set_multicast_opts(int sock_fd, char *ip)
{
    // join multicast group
    int opt = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt failed");
        return -1;
    }
    struct ip_mreq mreq;
    if (inet_pton(AF_INET, ip, &mreq.imr_multiaddr.s_addr) != 1)
    {
        perror("invalid ip address.");
        return -1;
    }
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt failed");
        return -1;
    }
    return 0;
}

int send_udp(int fd, int port, const char *dest, const char *msg_buffer, int msg_len)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, dest, &addr.sin_addr) != 1)
    {
        perror("invalid ip address.");
        return -1;
    }
    return sendto(fd, msg_buffer, msg_len, 0, (struct sockaddr *)&addr, sizeof(addr));
}
