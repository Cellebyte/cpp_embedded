/*
 *  @author Marcel Fest
 */
#include "Communication.h"
#define BUFSIZE 2048
CREATEPOOL(pool_2048,10,2048);
bool ClientServer::Start( Mode mode , unsigned int port , const char* ip )
{
    if(mode == Server)
    {

        int service_socket;              /* listening socket providing service */
        socklen_t address_length;        /* length of address structure */
        struct sockaddr_in host_addr;    /* address of this service */
        struct sockaddr_in client_addr;  /* client's address */
        int message_length = 0;

        if ((service_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            return false;
        }

        /* INADDR_ANY is the special IP address 0.0.0.0 which binds the */
        /* transport endpoint to all IP addresses on the machine. */

        memset((char*)&host_addr, 0, sizeof(host_addr)); //set structure to zero
        host_addr.sin_family = AF_INET;
        host_addr.sin_port = htons(port);
        host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        /* bind to the address to which the service will be offered */
        if (bind(service_socket, (struct sockaddr *)&host_addr, sizeof(host_addr)) < 0)
        {
            return false;
        }
        char inet_address[INET_ADDRSTRLEN];
        if(!inet_ntop(AF_INET, &(host_addr.sin_addr), inet_address, INET_ADDRSTRLEN))
        {
            return false;
        }
        printf("server started on %s, listening on port %d\n",inet_address, port);
        for (;;)
        {
            //#DEBUG printf("waiting on port %d\n", PORT);
            void* buf = pool_2048.Allocate(2000);
            message_length = recvfrom(host_addr, buf, BUFSIZE, 0, (struct sockaddr_in *)&client_addr, &address_length);
            printf("received %d bytes\n", message_length);
            if (message_length < 63)
            {
                printf("Message to short.\n");
            }
        }
            /* never exits */
    }else
    if(mode == Client)
    {
        struct sockaddr_in server_address;
        if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        //AF_INET = IPv4, SOCK_DGRAM = Datagram Socket, Protocol_Type = 0
        {
            return false;
        }
        memset((char*)&server_address,0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        inet_pton(AF_INET, ip, &(server_address.sin_addr));
        server_address.sin_port = htons(port);
        if (bind(udp_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
        {
            return false;
        }
        CREATESTRING(string_store, 20);
        string_store = "abcdef";
        if (sendto(fd, string_store, string_store.GetLength, 0, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        {
            return false;
        }
    }else return false;
}
