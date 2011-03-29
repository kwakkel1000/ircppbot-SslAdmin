#include "include/Session.h"
#include "include/AdminData.h"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
//#include <boost/asio/ssl.hpp>

/*Session::Session(boost::asio::io_service& io_service, boost::asio::ssl::context& context) : ssl_socket_(io_service, context)
{
	ssl=true;
}*/

Session::Session(boost::asio::io_service& io_service) : non_ssl_socket_(io_service)
{
	ssl=false;
}

/*ssl_socket::lowest_layer_type& Session::socket()
{
	return ssl_socket_.lowest_layer();
}*/

boost::asio::ip::tcp::socket& Session::non_ssl_socket()
{
	return non_ssl_socket_;
}

void Session::start()
{
	if (ssl)
	{
		/*ssl_socket_.async_handshake(boost::asio::ssl::stream_base::server,
									boost::bind(&Session::handle_handshake, this, boost::asio::placeholders::error));*/
	}
	else
	{
		non_ssl_socket_.async_read_some(boost::asio::buffer(data_, max_length),
										boost::bind(&Session::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
}

/*void Session::handle_handshake(const boost::system::error_code& error)
{
	if (!error)
	{
		ssl_socket_.async_read_some(boost::asio::buffer(data_, max_length),
								boost::bind(&Session::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ));
	}
	else
	{
		delete this;
	}
}*/

void Session::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{
		std::string buf_data = std::string(data_);
		std::cout << buf_data << std::endl;
		AdminData::Instance().AddRecvQueue(buf_data);
		if(ssl)
		{
			/*boost::asio::async_write(ssl_socket_, boost::asio::buffer(data_, bytes_transferred),
										boost::bind(&Session::handle_write, this, boost::asio::placeholders::error));*/
		}
		else
		{
			boost::asio::async_write(non_ssl_socket_, boost::asio::buffer(data_, bytes_transferred),
										boost::bind(&Session::handle_write, this, boost::asio::placeholders::error));
		}
	}
	else
	{
		delete this;
	}
}

void Session::handle_write(const boost::system::error_code& error)
{
	if (!error)
	{
		if (ssl)
		{
			/*ssl_socket_.async_read_some(boost::asio::buffer(data_, max_length),
										boost::bind(&Session::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));*/
		}
		else
		{
			non_ssl_socket_.async_read_some(boost::asio::buffer(data_, max_length),
											boost::bind(&Session::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}
	}
	else
	{
		delete this;
	}
}
