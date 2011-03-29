#ifndef SslAdmin_h
#define SslAdmin_h

#include <interfaces/AdminInterface.h>
#include <core/Parse.h>
#include "Server.h"

class SslAdmin : public AdminInterface
{
public:
    void Init(Parse *parse, int port);
    void Run();

private:
	Parse* mpParse;
	int mPort;

	Server* mpServer;
	boost::asio::io_service io_service;
};

#endif // SslAdmin_h

