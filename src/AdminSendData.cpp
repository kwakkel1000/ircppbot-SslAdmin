#include "include/AdminSendData.h"
#include <iostream>

AdminSendData::AdminSendData()
{
}

AdminSendData::~AdminSendData()
{
    stop();
}

void AdminSendData::stop()
{
    send = false;
    while (!SendQueue.empty())
    {
        SendQueue.pop();
    }
    SendAvailable.notify_all();
}

void AdminSendData::init()
{
    while (!SendQueue.empty())
    {
        SendQueue.pop();
    }
    send = true;
    assert(!send_thread);
    send_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&AdminSendData::SessionSend, this)));
}

void AdminSendData::AddSession(Session* s)
{
	SessionVector.push_back(s);
}

void AdminSendData::DelSession(Session *s)
{
    unsigned int session_it;
    std::cout << "SessionVector.size() " << SessionVector.size() << std::endl;
    for (session_it = SessionVector.size(); session_it > 0; session_it--)
    {
		std::cout << "session_it " << session_it-1 << std::endl;
        if (SessionVector[session_it-1] == s)
        {
        	std::cout << "session removed" << std::endl;
            SessionVector.erase(SessionVector.begin() + session_it-1);
        }
    }
}

void AdminSendData::AddSendQueue(std::string data)
{
    boost::mutex::scoped_lock lock(SendMutex);
    SendQueue.push(data);
    SendAvailable.notify_one();
}

std::string AdminSendData::GetSendQueue()
{
    boost::mutex::scoped_lock lock(SendMutex);
    std::string temp = "";
    while(SendQueue.empty())
    {
        SendAvailable.wait(lock);
    }
    if (send == true)
    {
        if (!SendQueue.empty())
        {
            temp = SendQueue.front();
            SendQueue.pop();
            return temp;
        }
    }
    return temp;
}



void AdminSendData::SessionSend()
{
    while(send)
    {
        std::string data;
        data = GetSendQueue();
        data = data + "\r\n";
        unsigned int session_it;
        for (session_it = 0; session_it < SessionVector.size(); session_it++)
        {
            SessionVector[session_it]->Write(data);
        }
    }
}
