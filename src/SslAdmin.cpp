#include "include/SslAdmin.h"
#include <iostream>

extern "C" AdminInterface* create()
{
    return new SslAdmin;
}

extern "C" void destroy(AdminInterface* x)
{
    delete x;
}

void SslAdmin::Init(Parse *parse, int port)
{
    std::cout << "SslAdmin::Init" << std::endl;
    mpParse=parse;
    mPort=port;

}

void SslAdmin::Run()
{
    mpServer = new Server(io_service, mPort);
    io_service.run();
}
