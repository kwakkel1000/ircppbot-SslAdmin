#ifndef Session_h
#define Session_h

#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
//#include <boost/asio/ssl.hpp>


//typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

class Session
{
public:

	//Session(boost::asio::io_service& io_service, boost::asio::ssl::context& context);
	Session(boost::asio::io_service& io_service);

	//ssl_socket::lowest_layer_type& socket();
	boost::asio::ip::tcp::socket& non_ssl_socket();

	void start();

	//void handle_handshake(const boost::system::error_code& error);

	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error);

private:
	bool ssl;
	//ssl_socket ssl_socket_;
	boost::asio::ip::tcp::socket non_ssl_socket_;
	enum { max_length = 1024 };
	char data_[max_length];
};

#endif // Session_h
