#include <errno.h>
#include "ssdp.h"
#include "net.h"
#include "utils.h"

char *ssdp_msg_type_to_string(ssdp_msg_type type)
{
    switch (type)
    {
    case M_SEARCH:
        return "M-SEARCH";
    case M_NOTIFY:
        return "NOTIFY";
    default:
        return "UNKNOWN";
    }
}

int validate_ssdp_msg(ssdp_msg *msg)
{
    if (!msg)
    {
        return -1;
    }

    if (!msg->host || !msg->man || !msg->st)
    {
        return -1;
    }

    return 0;
}

int ssdp_to_string(ssdp_msg *msg, char *buffer, size_t buffer_len)
{
    if (!msg || !buffer || buffer_len < 1)
    {
        return -1;
    }
    if (validate_ssdp_msg(msg) < 0)
    {
        return -1;
    }
    int result = snprintf(buffer, buffer_len, "%s * HTTP/1.1\r\n"
                                              "HOST: %s:1900\r\n"
                                              "MAN: \"%s\"\r\n"
                                              "MX: %d\r\n"
                                              "ST: %s\r\n"
                                              "\r\n",
                          ssdp_msg_type_to_string(msg->type), msg->host, msg->man, msg->mx, msg->st);

    if (result < 0 || result >= (int)buffer_len)
    {
        return -1;
    }
    return result;
}

int ssdp_send(int socket, ssdp_msg *msg)
{
    if (socket == NO_SOCKET)
    {
        perror("Invalid socket passed to ssdp_send");
        return 1;
    }

    char *buffer = malloc(SSDP_BUFFER_SIZE);
    if (ssdp_to_string(msg, buffer, SSDP_BUFFER_SIZE) < 0)
    {
        free(buffer);
        return -1;
    }

    int bytes_sent = send_udp(socket, SSDP_PORT, SSDP_MULTICAST_IP, buffer, strlen(buffer));
    if (bytes_sent < 0)
    {
        free(buffer);
        perror("sendto failed");
        return -1;
    }
    free(buffer);
    return 0;
}

int parse_ssdp_response(char *response_string, ssdp_response *response)
{
    split_r *tokens = split(response_string, "\r\n", 100);
    if (!tokens)
        return -1;
    for (int i = 0; i < tokens->count; i++)
    {
        printf("Token: %s\n", tokens->tokens[i]);
    }
    return tokens->count;
}

int receive_ssdp_msg(int socket, ssdp_response *response_array[MAX_DEVICES])
{
    int number_of_devices = 0;
    char buffer[SSDP_BUFFER_SIZE];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    (void)response_array;

    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (number_of_devices < MAX_DEVICES)
    {
        int bytes_received = recvfrom(socket, buffer, SSDP_BUFFER_SIZE, 0, (struct sockaddr *)&addr, &addr_len);

        printf("Received: %d bytes\n", bytes_received);
        printf("Found %d devices\n", number_of_devices);

        if (bytes_received < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            return -1;
        }

        ssdp_response *response = malloc(sizeof(ssdp_response));

        if (response == NULL)
        {
            return -1;
        }

        if (parse_ssdp_response(buffer, response) < 0)
        {
            continue;
        }

        response_array[number_of_devices++] = response;
        buffer[bytes_received] = '\0';
        printf("Received: %s\n", buffer);
    }

    return number_of_devices;
}
