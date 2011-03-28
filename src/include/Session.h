#ifndef Session_h
#define Session_h

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>


typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

class Session
{
public:

	Session(boost::asio::io_service& io_service, boost::asio::ssl::context& context);
	ssl_socket::lowest_layer_type& socket();

	void start();

	void handle_handshake(const boost::system::error_code& error);
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error);

private:
	ssl_socket socket_;
	enum { max_length = 1024 };
	char data_[max_length];
};

#endif // Session_h
