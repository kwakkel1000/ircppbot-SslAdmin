#ifndef AdminRecvData_H
#define AdminRecvData_H

#include <queue>
#include <string>

#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

class AdminRecvData
{
public:
	static AdminRecvData& Instance()
	{
		static AdminRecvData obj;
		return obj;
	}
    void init();
    void stop();

    //queue functions
    std::string GetRecvQueue();
    void AddRecvQueue(std::string data);

private:
    AdminRecvData();
    ~AdminRecvData();

    //vars
    bool recv;

    boost::condition RecvAvailable;

    boost::mutex RecvMutex;

    //admin queues
    std::queue< std::string > RecvQueue;

};

#endif // AdminRecvData_h

