#include "include/SslAdmin.h"
#include "include/AdminRecvData.h"
#include "include/AdminSendData.h"

#include <boost/algorithm/string.hpp>
#include <iostream>

extern "C" AdminInterface* create()
{
    return new SslAdmin;
}

extern "C" void destroy(AdminInterface* x)
{
    delete x;
}

void SslAdmin::Init(Bot *bot, int port)
{
    std::cout << "SslAdmin::Init" << std::endl;
    mpBot=bot;
    mPort=port;
    AdminRecvData::Instance().init();
    AdminSendData::Instance().init();
    assert(!parse_thread);
    parse_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&SslAdmin::ParseData, this)));
}

void SslAdmin::Run()
{
    mpServer = new Server(io_service, mPort);
    io_service.run();
}

void SslAdmin::ParseData()
{
	std::cout << "void SslAdmin::ParseData()" << std::endl;
    while(1)
    {
    	std::string recvd = AdminRecvData::Instance().GetRecvQueue();
    	std::cout << "void SslAdmin::ParseData(): " << recvd << std::endl;

		std::vector< std::string > split_data;
		std::vector< std::string > args;
		std::string command;
		boost::split( split_data, recvd, boost::is_any_of(" "), boost::token_compress_on );
		if (split_data.size() >= 1)
		{
			std::cout << "command ";
			command = split_data[0];
			std::cout << command << std::endl;
		}
		if (split_data.size() >= 2)
		{
			std::cout << "args";
			for (unsigned int i = 1; i < split_data.size(); i++)
			{
				args.push_back(split_data[i]);
				std::cout << " [" << split_data[i] << "]";
			}
			std::cout << std::endl;
		}
		mpBot->AdminCommands(command, args);
    }
}


void SslAdmin::AddSendQueue(std::string mSendData)
{
	std::cout << mSendData << std::endl;
	AdminSendData::Instance().AddSendQueue(mSendData);
}
