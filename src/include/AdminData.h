#ifndef AdminData_H
#define AdminData_H

#include <queue>
#include <vector>
#include <string>

#include <boost/bind.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

class AdminData
{
public:
	static AdminData& Instance()
	{
		static AdminData obj;
		return obj;
	}
    void init();
    void stop();

    //queue functions
    std::string GetSendQueue();
    std::string GetRecvQueue();
    void AddRecvQueue(std::string data);
    void AddSendQueue(std::string data);

private:
    AdminData();
    ~AdminData();

    //vars
    bool recv;
    bool send;

    boost::condition SendAvailable;
    boost::condition RecvAvailable;

    boost::mutex SendMutex;
    boost::mutex RecvMutex;

    //admin queues
    std::queue< std::string > SendQueue;
    std::queue< std::string > RecvQueue;

};

#endif // AdminData_h

