#ifndef SOCKET_TCP_H
#define SOCKET_TCP_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

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

struct server_callback
{
    void (* success)(int);
    void (* fail)(int);
    void * props;
};

//Creates a new tcp IPv4 Server.
struct socket_info tcpIPv4Server(char * IPAddr, int port);
struct socket_info startTcpServer(struct socket_info info, struct server_callback callbacks, int request_limit, int time_out);


#endif