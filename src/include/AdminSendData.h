//
//
//  @ Project : ircppbot
//  @ File Name : Bot.cpp
//  @ Date : 4/18/2011
//  @ Author : Gijs Kwakkel
//
//
// Copyright (c) 2011 Gijs Kwakkel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//

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

