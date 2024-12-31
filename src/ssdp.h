#ifndef BEAM_SSDP_H
#define BEAM_SSDP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "net.h"

#define SSDP_PORT 1900
#define SSDP_MULTICAST_IP "239.255.255.250"
#define SSDP_BUFFER_SIZE 1024
#define MAX_DEVICES 100
#define MAX_SEARCH_DURATION 60

typedef enum ssdp_msg_type
{
    M_SEARCH,
    M_NOTIFY,
} ssdp_msg_type;

char *ssdp_msg_type_to_string(ssdp_msg_type type);

typedef struct
{
    ssdp_msg_type type;
    char *host;
    char *man;
    int mx;
    char *st;
} ssdp_msg;

typedef struct
{
    char location[256];
    char usn[256];
    char device_type[64];
    char *server;
    int ttl;
} ssdp_response;

int validate_ssdp_msg(ssdp_msg *msg);
int ssdp_to_string(ssdp_msg *msg, char *buffer, size_t buffer_len);
int ssdp_send(int socket, ssdp_msg *msg);
int receive_ssdp_msg(int socket, ssdp_response *response_array[MAX_DEVICES]);
int parse_ssdp_response(char *response_string, ssdp_response *response);

#endif
