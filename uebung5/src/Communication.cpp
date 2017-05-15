/*
 *  @author Marcel Fest
 */
#include "Communication_real.h"
bool ClientServer::Start( Mode mode , unsigned int port , const char* ip)
{
    ClientServer_Real client_server;
    return client_server.Start(static_cast<ClientServer_Real::Mode>(mode),port,ip);
}
