#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "../TCP/socket_tcp.h"
#include <stdbool.h>
#include <pthread.h>

#define RESOURCES_BLOCK 10
#define END_POINT_SIZE 16
#define FILE_BUFFER 1024

struct resource
{
    char * path;
    char * name;
    char * type;
};

struct http_server
{
    struct socket_info info;
    pthread_t * server_thread;
    char * IP;
    char end_pont[END_POINT_SIZE+1];
    int port, resources_length;
    struct resource * resources;
};

struct http_server * startHttpServer(char *IP, int port);
bool addResource(struct http_server * server, char * path);
void stopHttpServer(struct http_server * server);

#endif