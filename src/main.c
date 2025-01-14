#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "net.h"
#include "ssdp.h"
#include "../include/raylib.h"

#define MAX_RETRY_COUNT 10
#define RETRY_DELAY_SEC 1
#define SHOULD_RETRY 0

int UDP_SOCKET = NO_SOCKET;

void handle_signal(int sig)
{
    if (UDP_SOCKET != NO_SOCKET)
    {
        close(UDP_SOCKET);
    }
    exit(EXIT_SUCCESS);
}

int main(void)
{
    InitWindow(800, 600, "BEAM");
    int retry_count = 0;
    int res;
    UDP_SOCKET = init_udp_socket();
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    res = set_multicast_opts(UDP_SOCKET, SSDP_MULTICAST_IP);
    if (res < 0)
    {
        perror("set multicast opts failed");
        close(UDP_SOCKET);
        exit(EXIT_FAILURE);
    }
    ssdp_msg msg = {
        .type = M_SEARCH,
        .mx = 3,
        .host = SSDP_MULTICAST_IP,
        .man = "ssdp:discover",
        .st = "ssdp:all",
    };

    res = ssdp_send(UDP_SOCKET, &msg);
    if (res < 0)
    {
        if (retry_count < MAX_RETRY_COUNT && SHOULD_RETRY)
        {
            retry_count++;
            sleep(RETRY_DELAY_SEC);
            res = ssdp_send(UDP_SOCKET, &msg);
        }
        else
        {
            perror("ssdp_send failed");
            close(UDP_SOCKET);
            exit(EXIT_FAILURE);
        }
    }
    printf("listening for responses\n");
    ssdp_response *responses[MAX_DEVICES];
    receive_ssdp_msg(UDP_SOCKET, responses);

    // for (int i = 0; i < MAX_DEVICES; i++)
    // {
    //     if (responses[i])
    //     {
    //         printf("Device: %s\n", responses[i]->usn);
    //     }
    // }

    close(UDP_SOCKET);
    CloseWindow();
    return 0;
}
