/*
 *  @author Marcel Fest
 */
#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <sys/socket.h>
#include "Server.h"
#include "Client.h"
#include "PoolAllocator.h"

class ClientServer
{

    public:
        enum Mode : unsigned int
        {
            Server,
            Client
        };

        bool Start ( Mode mode , unsigned int port , const char * ip );
};

#endif
