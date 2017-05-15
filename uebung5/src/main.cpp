/*
 *  @author Marcel Fest
 */

#include "Communication.h"
#include <cstdio>

int main()
{
    printf("I am working\n");
    ClientServer client;
    //ClientServer server;
    //if(!server.Start(ClientServer::Mode::Server,12345,"localhost"))return 1;
    if(!client.Start(ClientServer::Mode::Client,12345,"127.0.0.1"))return 1;
}
