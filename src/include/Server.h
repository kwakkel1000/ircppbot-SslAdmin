#ifndef Server_h
#define Server_h

#include "Session.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>

class Server
{
public:
  Server(boost::asio::io_service& io_service, unsigned short port);
  void handle_accept(Session* new_session, const boost::system::error_code& error);

private:
  boost::asio::io_service& io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
};

#endif  // Server_h

