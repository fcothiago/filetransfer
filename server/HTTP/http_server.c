#include "http_server.h"
#include <stdlib.h>

void * process_request(void * args)
{
    printf("Hello World\n");
}

void * http_server_job(void * argc)
{
    struct http_server * server = (struct http_server *) argc;
    server->info = tcpIPv4Server(server->IP, server->port);
    startTcpServer(server->info, process_request, NULL, 10, 1000);
}

struct http_server * startHttpServer(char *IP, int port)
{
    struct http_server * server = (struct http_server *) malloc(sizeof(server));
    server->IP = IP;
    server->port = port;
    server->resources_length = 0;
    server->resources_paths = (char **) malloc(RESOURCES_BLOCK*sizeof(char *));
    server->server_thread = (pthread_t *) malloc(sizeof(pthread_t));
    pthread_create(server->server_thread,NULL,http_server_job,server);
    return server;
}