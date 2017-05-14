/*
 *  @author Marcel Fest
 */

#include "Communication.h"
#include <cstdio>

int main()
{
    printf("I am working\n");
    ClientServer client,server;
    server.Start(Server,12345,"localhost");
    client.Start(Client,12345,"localhost");
}
