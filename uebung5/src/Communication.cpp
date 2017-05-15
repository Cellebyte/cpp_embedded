/*
 *  @author Marcel Fest
 */
#include "Communication.h"
#define BUFSIZE 1500
CREATEPOOL(packet_pool,10,BUFSIZE);
bool ClientServer::Start( Mode mode , unsigned int port , const char* ip )
{
    if ( !port || nullptr == ip || ! mode)return false;

    if(mode == ClientServer::Mode::Server)
    {

        int service_socket;              /* listening socket providing service */
        struct sockaddr_in host_addr;    /* address of this service */
        struct sockaddr_in client_addr;  /* client's address */
        socklen_t address_length = sizeof(client_addr);       /* length of address structure */
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
        printf("server started on %s, listening on port %u\n",inet_address, port);
        for (;;)
        {
            //#DEBUG printf("waiting on port %d\n", PORT);
            void* buf = packet_pool.Allocate(1500);
            message_length = recvfrom(service_socket, buf, BUFSIZE, 0, (sockaddr *)&client_addr, &address_length);
            printf("received %d bytes\n", message_length);
            if (message_length < 6)
            {
                printf("Message to short.\n");
            }
        }
            /* never exits */
    }else
    if(mode == ClientServer::Mode::Client)
    {
        struct sockaddr_in myaddr;
        struct sockaddr_in remaddr;
        int udp_socket = 0;
        if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        //AF_INET = IPv4, SOCK_DGRAM = Datagram Socket, Protocol_Type = 0
        {
            return false;
        }
        memset((char *)&myaddr, 0, sizeof(myaddr));
    	myaddr.sin_family = AF_INET;
    	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    	myaddr.sin_port = htons(0);

        memset((char *) &remaddr, 0, sizeof(remaddr));
        remaddr.sin_family = AF_INET;
        remaddr.sin_port = htons(port);

        if (bind(udp_socket, (sockaddr *)&myaddr, sizeof(myaddr)) < 0)
        {
            perror("bind failed");
            return false;
        }
        char string_store [] = "abcdef";
        if (sendto(udp_socket, string_store, 7, 0, (sockaddr *)&remaddr, sizeof(remaddr)) < 0) return false;
    }else return false;
    return true;
}
