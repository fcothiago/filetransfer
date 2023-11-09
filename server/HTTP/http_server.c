#include "http_server.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DIGIT_OR_LETTER(C) ( ('a' <= C && C <= 'z') || ('A' <= C && C <= 'Z') || ('0' <= C && C <= '9'))
#define END "\r\n"

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

void post_file(int socket_descriptor, struct resource rsc)
{
    char response_header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/";
    char filename_header[] = "\r\nContent-Disposition: attachment; filename=\"";
    char finish[] = "\"\r\n\r\n";

    const int total_size = sizeof(response_header) + sizeof(filename_header) + sizeof(finish) + 1 + 2*strlen(rsc.type) + strlen(rsc.name) + 1;
    char * msg = (char *) malloc(sizeof(char)*total_size);
    msg[0] = '\0';

    strcat(msg,response_header);
    strcat(msg,rsc.type);
    strcat(msg,filename_header);
    strcat(msg,rsc.name);
    strcat(msg,".");
    strcat(msg,rsc.type);
    strcat(msg,finish);

    send(socket_descriptor,msg,total_size - 1,0);

    FILE * file = fopen(rsc.path,"r");
    if(file)
    {
        rewind(file);
        char buffer[FILE_BUFFER];
        int count;
        do
        {
            count = fread(buffer,sizeof(char),FILE_BUFFER,file);
            send(socket_descriptor,buffer,sizeof(char)*count,0);
        }while (count >= FILE_BUFFER);
    }
    fclose(file);
}

void * process_request(void * args)
{ 
    struct callback_args * cb_args = ( struct callback_args * ) args;
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(cb_args->socket_descriptor, buffer, BUFFER_SIZE, 0);
    char * end_point = get_end_point(buffer);
    for(;bytes_received >= BUFFER_SIZE;)
    {
        bytes_received = recv(cb_args->socket_descriptor, buffer, BUFFER_SIZE, 0);
    }
    struct http_server * server = cb_args->args;
    for(int i = 0; i < server->resources_length; i++)
    {
        post_file(cb_args->socket_descriptor,server->resources[i]);
    }
    close(cb_args->socket_descriptor);
}

void * http_server_job(void * argc)
{
    struct http_server * server = (struct http_server *) argc;
    server->info = tcpIPv4Server(server->IP, server->port);
    startTcpServer(server->info, process_request, server, 10, 1000);
}

struct http_server * startHttpServer(char *IP, int port)
{
    struct http_server * server = (struct http_server *) malloc(sizeof(struct http_server));
    server->IP = IP;
    server->port = port;
    server->resources_length = 0;
    server->resources = (struct resource *) malloc(sizeof(struct resource ));
    server->server_thread = (pthread_t *) malloc(sizeof(pthread_t));
    pthread_create(server->server_thread,NULL,http_server_job,server);
    return server;
}