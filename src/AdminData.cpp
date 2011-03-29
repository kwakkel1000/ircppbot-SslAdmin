#include "include/AdminData.h"
AdminData::AdminData()
{
}

AdminData::~AdminData()
{
    stop();
}

void AdminData::stop()
{
    send = false;
    recv = false;
    while (!SendQueue.empty())
    {
        SendQueue.pop();
    }
    while (!RecvQueue.empty())
    {
        RecvQueue.pop();
    }
    SendAvailable.notify_all();
    RecvAvailable.notify_all();
}

void AdminData::init()
{
    while (!SendQueue.empty())
    {
        SendQueue.pop();
    }
    while (!RecvQueue.empty())
    {
        RecvQueue.pop();
    }
    send = true;
    recv = true;
}


void AdminData::AddSendQueue(std::string data)
{
    boost::mutex::scoped_lock lock(SendMutex);
    SendQueue.push(data);
    SendAvailable.notify_one();
}

std::string AdminData::GetSendQueue()
{
    boost::mutex::scoped_lock lock(SendMutex);
    std::string temp = "";
    while(SendQueue.empty() && send == true)
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

void AdminData::AddRecvQueue(std::string data)
{
    boost::mutex::scoped_lock lock(RecvMutex);
    RecvQueue.push(data);
    RecvAvailable.notify_one();
}

std::string AdminData::GetRecvQueue()
{
    boost::mutex::scoped_lock lock(RecvMutex);
    std::string temp = "";
    while(RecvQueue.empty() && recv == true)
    {
        RecvAvailable.wait(lock);
    }
    if (recv == true)
    {
        temp = RecvQueue.front();
        RecvQueue.pop();
    }
    return temp;
}

