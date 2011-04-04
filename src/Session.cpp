#define NOSSL
//#define SSL

#include "include/Session.h"
#include "include/AdminRecvData.h"
#include "include/AdminSendData.h"
#include <cstdlib>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#ifdef SSL
#include <boost/asio/ssl.hpp>

Session::Session(boost::asio::io_service& io_service, boost::asio::ssl::context& context) : ssl_socket_(io_service, context)
{
	ssl=true;
}
#endif

Session::Session(boost::asio::io_service& io_service) : non_ssl_socket_(io_service)
{
	ssl=false;
	UseAuth = true;
}


Session::~Session()
{
	AdminSendData::Instance().DelSession(this);
	std::cout << "close session" << std::endl;
}

#ifdef SSL
ssl_socket::lowest_layer_type& Session::socket()
{
	return ssl_socket_.lowest_layer();
}
#endif

boost::asio::ip::tcp::socket& Session::non_ssl_socket()
{
	return non_ssl_socket_;
}

void Session::start()
{
	PasswordAuth = -1;
	UsernameAuth = -1;
	if (UseAuth)
	{
		AuthStart();
	}
	else
	{
		AdminSendData::Instance().AddSession(this);
	}
	std::cout << "start read thread" << std::endl;
	//start read thread
    assert(!read_thread);
    read_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Session::read, this)));
}

#ifdef SSL
void Session::handle_handshake(const boost::system::error_code& error)
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
}
#endif

void Session::read()
{
	//start to receive
	memset( data_, '\0', max_length );
	if (ssl)
	{
#ifdef SSL
		ssl_socket_.async_read_some(boost::asio::buffer(data_, max_length),
									boost::bind(&Session::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
#endif
	}
	else
	{
		non_ssl_socket_.async_read_some(boost::asio::buffer(data_, max_length),
										boost::bind(&Session::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
}

void Session::Write(std::string data)
{
	char write_data_[max_length];
	memset( write_data_, '\0', max_length );
	strcpy(write_data_, data.c_str());
	size_t request_length = std::strlen(write_data_);
	if (ssl)
	{
#ifdef SSL
		boost::asio::write(ssl_socket_, boost::asio::buffer(write_data_, request_length));
#endif
	}
	else
	{
		boost::asio::write(non_ssl_socket_, boost::asio::buffer(write_data_, request_length));
	}
}



void Session::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
	try
	{
		if (error == boost::asio::error::eof)
		{
			std::cout << "closed by client" << std::endl;
			delete this;
			return;
		}
		else if (error)
		{
			throw boost::system::system_error(error); // Some other error.
			delete this;
			return;
		}
		else
		{
			std::string buf_data = std::string(data_);
			boost::trim(buf_data);
			std::cout << "void Session::read() " << buf_data << std::endl;
			if (!UseAuth || PasswordAuth == 1)
			{
				AdminRecvData::Instance().AddRecvQueue(buf_data);
			}
			if (UseAuth)
			{
				if (UsernameAuth != -1 && PasswordAuth == -1)
				{
					char write_data_[max_length];
					memset( write_data_, '\0', max_length );
					if(true)	//data == password bla bla
					{
						std::string send_string = "welcome\r\n";
						std::cout << "send_string: " << send_string << std::endl;
						strcpy(write_data_, send_string.c_str());
						size_t request_length = std::strlen(write_data_);
						if (ssl)
						{
#ifdef SSL
							boost::asio::write(ssl_socket_, boost::asio::buffer(write_data_, request_length));
#endif
						}
						else
						{
							boost::asio::write(non_ssl_socket_, boost::asio::buffer(write_data_, request_length));
						}
						PasswordAuth = 1;
						AdminSendData::Instance().AddSession(this);
					}
					else
					{
						std::string send_string = "wrong username or password\r\n";
						std::cout << "send_string: " << send_string << std::endl;
						strcpy(write_data_, send_string.c_str());
						size_t request_length = std::strlen(write_data_);
						if (ssl)
						{
#ifdef SSL
							boost::asio::write(ssl_socket_, boost::asio::buffer(write_data_, request_length));
#endif
						}
						else
						{
							boost::asio::write(non_ssl_socket_, boost::asio::buffer(write_data_, request_length));
						}
						PasswordAuth = 0;
						delete this;
						return;
					}
				}
				if (UsernameAuth == -1)
				{
					if(true)	//data == username bla bla
					{
						UsernameAuth = 1;
					}
					else
					{
						UsernameAuth = 0;
					}
					char write_data_[max_length];
					memset( write_data_, '\0', max_length );
					std::string send_string = "password=";
					std::cout << "send_string: " << send_string << std::endl;
					strcpy(write_data_, send_string.c_str());
					size_t request_length = std::strlen(write_data_);
					if (ssl)
					{
#ifdef SSL
						boost::asio::write(ssl_socket_, boost::asio::buffer(write_data_, request_length));
#endif
					}
					else
					{
						boost::asio::write(non_ssl_socket_, boost::asio::buffer(write_data_, request_length));
					}
				}
			}
			memset( data_, '\0', max_length );
			if (ssl)
			{
#ifdef SSL
				ssl_socket_.async_read_some(boost::asio::buffer(data_, max_length),
											boost::bind(&Session::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
#endif
			}
			else
			{
				non_ssl_socket_.async_read_some(boost::asio::buffer(data_, max_length),
												boost::bind(&Session::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

void Session::handle_write(const boost::system::error_code& error)
{
	if (!error)
	{
		/*char write_data_[max_length];
		memset( write_data_, '\0', max_length );
		std::string send_string = AdminData::Instance().GetSendQueue();
		send_string = send_string + "\r\n";
		strcpy(write_data_, send_string.c_str());
		size_t request_length = std::strlen(write_data_);
		if (ssl)
		{
#ifdef SSL
			boost::asio::async_write(ssl_socket_, boost::asio::buffer(write_data_, request_length),
										boost::bind(&Session::handle_write, this, boost::asio::placeholders::error));
#endif
		}
		else
		{
			boost::asio::async_write(non_ssl_socket_, boost::asio::buffer(write_data_, request_length),
										boost::bind(&Session::handle_write, this, boost::asio::placeholders::error));
		}*/
	}
	else
	{
		delete this;
	}
}



void Session::AuthStart()
{
	char write_data_[max_length];
	memset( write_data_, '\0', max_length );
	std::string send_string = "username=";
	std::cout << "send_string: " << send_string << std::endl;
	strcpy(write_data_, send_string.c_str());
	size_t request_length = std::strlen(write_data_);
	if (ssl)
	{
#ifdef SSL
		boost::asio::write(ssl_socket_, boost::asio::buffer(write_data_, request_length));
		memset( data_, '\0', max_length );
		ssl_socket_.async_read_some(boost::asio::buffer(data_, max_length),
									boost::bind(&Session::handle_username_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
#endif
	}
	else
	{
		boost::asio::write(non_ssl_socket_, boost::asio::buffer(write_data_, request_length));
		memset( data_, '\0', max_length );
		non_ssl_socket_.async_read_some(boost::asio::buffer(data_, max_length),
										boost::bind(&Session::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
}
