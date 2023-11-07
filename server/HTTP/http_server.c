#include "http_server.h"
#include <stdlib.h>
#include <string.h>

#define DIGIT_OR_LETTER(C) ( ('a' <= C && C <= 'z') || ('A' <= C && C <= 'Z') || ('0' <= C && C <= '9'))

char *  get_end_point(char * buffer)
{
    char * pointer = strstr(buffer, "GET /");
    char * end_point = NULL;
    int i;
    if(pointer)
    {
        end_point = malloc(END_POINT_SIZE*sizeof(char));
        end_point[0] = '/';
        pointer = &pointer[4];
        for(i = 1; i < END_POINT_SIZE && DIGIT_OR_LETTER(pointer[i]) ; i++)
        {
            end_point[i] = pointer[i];   
        }
        end_point[i] = '\0';
    }
    return end_point;
}

void * process_request(void * args)
{ 
    struct callback_args * cb_args = ( struct callback_args * ) args;
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(cb_args->socket_descriptor, buffer, BUFFER_SIZE, 0);
    char * c = get_end_point(buffer);
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