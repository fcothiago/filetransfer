#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "../TCP/socket_tcp.h"
#include <pthread.h>

#define RESOURCES_BLOCK 10

struct http_server
{
    struct socket_info info;
    pthread_t * server_thread;
    char ** resources_paths;
    char * IP;
    int resources_length, port;
};

struct http_server * startHttpServer(char *IP, int port);
void stopHttpServer(struct http_server * server);

#endif