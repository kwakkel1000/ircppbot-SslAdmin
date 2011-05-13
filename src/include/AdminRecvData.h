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

