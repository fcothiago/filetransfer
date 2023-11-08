#include "http_server.h"
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

bool addResource(struct http_server * server, char * path)
{
    struct resource rsc;
    char * bar_pointer = strrchr(path,'/');
    char * dot_pointer = strrchr(path,'.');
    char * zero_pointer = strrchr(path,'\0');
    if(bar_pointer == NULL || dot_pointer == NULL ||  server->resources_length >= RESOURCES_BLOCK )
        return false;
    int name_size = dot_pointer - bar_pointer - 1;
    int type_size = zero_pointer - dot_pointer - 1;
    rsc.name = (char *) malloc((name_size + 1)*sizeof(char));
    rsc.type = (char *) malloc((type_size + 1)*sizeof(char));
    rsc.path = path;
    int i = 0;
    for(i = 0; i < name_size ; i++)
        rsc.name[i] = bar_pointer[i+1];
    rsc.name[i] = '\0';
    for(i = 0; i < type_size ; i++)
        rsc.type[i] = dot_pointer[i+1];
    rsc.type[i] = '\0';
    server->resources[server->resources_length++] = rsc;
    return true;
}