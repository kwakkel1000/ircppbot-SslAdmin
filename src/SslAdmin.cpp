#include "include/SslAdmin.h"
#include "include/AdminData.h"

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

void SslAdmin::Init(Parse *parse, int port)
{
    std::cout << "SslAdmin::Init" << std::endl;
    mpParse=parse;
    mPort=port;
    AdminData::Instance().init();
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
    	std::string recvd = AdminData::Instance().GetRecvQueue();
    	std::cout << "void SslAdmin::ParseData(): " << recvd << std::endl;

		size_t triggerpos = std::string::npos;
		triggerpos = recvd.find("load");
		if (triggerpos != std::string::npos)
		{
			mpParse->LoadModule("Example");
		}
		triggerpos = recvd.find("unload");
		if (triggerpos != std::string::npos)
		{
			mpParse->UnLoadModule("Example");
		}
		triggerpos = recvd.find("listmodules");
		if (triggerpos != std::string::npos)
		{
			for (unsigned int i = 0; i < mpParse->modulelist.size(); i++)
			{
				std::string modname = mpParse->modulelist[i];
				std::cout << modname << std::endl;
			}
		}
    }
}
