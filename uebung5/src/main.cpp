/*
 *  @author Marcel Fest
 */

#include "Communication.h"
#include <cstdio>

int main()
{
    printf("I am working\n");
    ClientServer client/*,server*/;
    //server.Start(ClientServer::Mode::Server,12345,"localhost");
    if(!client.Start(ClientServer::Mode::Client,12345,"127.0.0.1"))return 1;
}
