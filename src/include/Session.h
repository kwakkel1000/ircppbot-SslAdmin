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

#ifndef Session_h
#define Session_h

#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
//#include <boost/asio/ssl.hpp>


//typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

class Session
{
public:

	//Session(boost::asio::io_service& io_service, boost::asio::ssl::context& context);
	Session(boost::asio::io_service& io_service);

	~Session();

	//ssl_socket::lowest_layer_type& socket();
	boost::asio::ip::tcp::socket& non_ssl_socket();

	void start();
	void Write(std::string);

private:

	//void handle_handshake(const boost::system::error_code& error);

	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

	//ssl_socket ssl_socket_;
	boost::asio::ip::tcp::socket non_ssl_socket_;
	enum { max_length = 1024 };
	char data_[max_length];
	void read();
	void AuthStart();

    boost::shared_ptr<boost::thread> read_thread;

	bool ssl;
    bool UseAuth;
    int PasswordAuth;
    int UsernameAuth;
};

#endif // Session_h
