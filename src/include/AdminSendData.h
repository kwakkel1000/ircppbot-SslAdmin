#ifndef AdminSendData_H
#define AdminSendData_H

#include <queue>
#include <vector>
#include <string>

#include "Session.h"

#include <boost/bind.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

class Session;
class AdminSendData
{
public:
	static AdminSendData& Instance()
	{
		static AdminSendData obj;
		return obj;
	}
    void init();
    void stop();
    void AddSession(Session *s);
    void DelSession(Session *s);

    //queue functions
    std::string GetSendQueue();
    void AddSendQueue(std::string data);

private:
    AdminSendData();
    ~AdminSendData();

    //vars
    bool send;

    boost::shared_ptr<boost::thread> send_thread;
    boost::condition SendAvailable;

    boost::mutex SendMutex;

    //admin queues
    std::queue< std::string > SendQueue;

    std::vector< Session * > SessionVector;

    void SessionSend();

};

#endif // AdminSendData_h

