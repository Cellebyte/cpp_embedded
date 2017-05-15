/*
 *  @author Marcel Fest
 */
#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "PoolAllocator.h"
#include "PreAllocString.h"

class ClientServer
{

    public:
        enum Mode : unsigned int
        {
            Server,
            Client
        };
        bool Start ( Mode mode , unsigned int port , const char* ip );
};


#endif
