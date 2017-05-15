/*
 *  @author Marcel Fest
 */
#include "Communication.h"
#define BUFSIZE 1500
CREATEPOOL(packet_pool,10,BUFSIZE);
bool ClientServer::Start( Mode mode , unsigned int port , const char* ip )
{
    if ( port >=65536 || nullptr == ip)
    {
        printf("problem\n");
        return false;
    }
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
        if (inet_pton(AF_INET, ip, &(host_addr.sin_addr)) < 0)
        {
            printf("No valid Address: %s\nUsing 0.0.0.0\n",ip);
            host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }

        /* bind to the address to which the service will be offered */
        if (bind(service_socket, (struct sockaddr *)&host_addr, sizeof(host_addr)) < 0)
        {
            printf("Bind to upd port failes\n");
            return false;
        }
        char inet_address[INET_ADDRSTRLEN];
        if(!inet_ntop(AF_INET, &(host_addr.sin_addr), inet_address, INET_ADDRSTRLEN))
        /* big endian address to string conversion */
        {
            printf("conversion from big endian to string failed\n");
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
            /* never exits --> check on keyboard interrupt*/
    }else
    if(mode == ClientServer::Mode::Client)
    {
        struct sockaddr_in client_address;      /* Client address struct*/
        struct sockaddr_in server_address;      /* Server address Struct*/
        int udp_socket = 0;
        if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        //AF_INET = IPv4, SOCK_DGRAM = Datagram Socket, Protocol_Type = 0
        {
            return false;
        }
        memset((char *)&client_address, 0, sizeof(client_address));
    	client_address.sin_family = AF_INET;
    	client_address.sin_addr.s_addr = htonl(INADDR_ANY);
    	client_address.sin_port = htons(0);                    /* 0 = use random free port for client*/

        memset((char *) &server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);          /* service port to big endian*/

        if (bind(udp_socket, (sockaddr *)&client_address, sizeof(client_address)) < 0)
        {
            perror("bind failed");
            return false;
        }
        char string_store [] = "abcdef";
        if (sendto(udp_socket, string_store, 7, 0, (sockaddr *)&server_address, sizeof(server_address)) < 0) return false;
    }else return false;
    return true;
}
