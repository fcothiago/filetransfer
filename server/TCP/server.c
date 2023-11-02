#include "socket_tcp.h"
#include <sys/poll.h>

struct socket_info tcpIPv4Server(char * IPAddr, int port)
{
    struct socket_info infos;
    struct sockaddr_in server_addr;
    int socket_result;
    int socket_options_value = 1;
    do
    {
        /*Creating Socket*/
        infos.socket_descriptor = socket(AF_INET,SOCK_STREAM,0);
        if(infos.socket_descriptor < 0)
        {
            infos.socket_status = SOCKET_CREATION_FAILED;
            break;
        }
        /*Setting Socket Options*/
        socket_result = setsockopt(infos.socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (char *) & socket_options_value, sizeof(socket_options_value));
        if(socket_result < 0)
        {
            infos.socket_status = SOCKET_CONFIG_FAILED;
            break;
        }
        /*Setting Server Address*/
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        inet_aton(IPAddr, &server_addr.sin_addr);
        /*Biding Server to IPAddr*/
        socket_result = bind(infos.socket_descriptor, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if(socket_result < 0)
        {
            infos.socket_status = SOCKET_BIND_FAILED;
            break;
        }
        infos.socket_status = SOCKET_CREATED;

    }while(0);
    return infos;
}

struct socket_info startTcpServer(struct socket_info info, struct server_callback callbacks, int request_limit, int time_out)
{
    int response, received_socket;
    struct pollfd fds;
    nfds_t nfds = 1;
    do
    {
        response = listen(info.socket_descriptor,request_limit);
        if(response < 0) 
            continue;;
        received_socket = accept(info.socket_descriptor, NULL, NULL);
        if(response < 0) 
            continue;
        //Waiting to available data.
        memset(&fds, 0, sizeof(fds));
        fds.fd = -1;
        fds.events = POLLIN;
        fds.revents = 0;
        response = poll(&fds, nfds, time_out);
        if(response == 0)
            callbacks.success(received_socket);
        else
            callbacks.fail(received_socket);
    }while (1);
    
}