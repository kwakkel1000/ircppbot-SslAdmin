#ifndef SslAdmin_h
#define SslAdmin_h

#include <core/Bot.h>
#include "Server.h"
#include <boost/thread/thread.hpp>
#include <interfaces/AdminInterface.h>

class SslAdmin : public AdminInterface
{
public:
    void Init(Bot *bot, int port);
    void Run();
    void AddSendQueue(std::string mSendData);

private:
	void ParseData();
    boost::shared_ptr<boost::thread> parse_thread;

	Bot* mpBot;
	int mPort;

	Server* mpServer;
	boost::asio::io_service io_service;
};

#endif // SslAdmin_h

