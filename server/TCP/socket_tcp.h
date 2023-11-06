#ifndef SOCKET_TCP_H
#define SOCKET_TCP_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define THREAD_POOL_SIZE 10

enum socket_flags
{
    SOCKET_CREATED,
    SOCKET_CREATION_FAILED,
    SOCKET_CONFIG_FAILED,
    SOCKET_BIND_FAILED
};

struct socket_info
{
    int socket_status;
    int socket_descriptor;
    int port;
};

struct callback_args
{
    int socket_descriptor;
    void * args;
};

//Creates a new tcp IPv4 Server.
struct socket_info tcpIPv4Server(char * IPAddr, int port);
struct socket_info startTcpServer(struct socket_info info, void * (* callback)(void *), void * args, int request_limit, int time_out);

#endif