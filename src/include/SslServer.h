#ifndef SslServer_h
#define SslServer_h

#include "Session.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

class SslServer
{
public:
  SslServer(boost::asio::io_service& io_service, unsigned short port);

  std::string get_password() const;

  void handle_accept(Session* new_session, const boost::system::error_code& error);

private:
  boost::asio::io_service& io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ssl::context context_;
};

#endif  // SslServer_h
