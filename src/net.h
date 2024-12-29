#ifndef BEAM_NET_H
#define BEAM_NET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#define sock_in struct sockaddr_in
#define NO_SOCKET -1

void signal_handler(int sock_fd);
int connect_udp(int port, char *ip, char *data, int data_len);
int init_udp_socket();
int send_udp(int fd, int port, const char *dest, const char *msg_buffer, int msg_len);
int set_multicast_opts(int sock_fd, char *ip);
#endif
