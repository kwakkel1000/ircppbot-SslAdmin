#include "include/AdminRecvData.h"
AdminRecvData::AdminRecvData()
{
}

AdminRecvData::~AdminRecvData()
{
    stop();
}

void AdminRecvData::stop()
{
    recv = false;
    while (!RecvQueue.empty())
    {
        RecvQueue.pop();
    }
    RecvAvailable.notify_all();
}

void AdminRecvData::init()
{
    while (!RecvQueue.empty())
    {
        RecvQueue.pop();
    }
    recv = true;
}

void AdminRecvData::AddRecvQueue(std::string data)
{
    boost::mutex::scoped_lock lock(RecvMutex);
    RecvQueue.push(data);
    RecvAvailable.notify_one();
}

std::string AdminRecvData::GetRecvQueue()
{
    boost::mutex::scoped_lock lock(RecvMutex);
    std::string temp = "";
    while(RecvQueue.empty())
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

