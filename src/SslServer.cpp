#include "include/SslServer.h"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

SslServer::SslServer(boost::asio::io_service& io_service, unsigned short port)
	: io_service_(io_service), acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), context_(io_service, boost::asio::ssl::context::sslv23)
{
	context_.set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::no_sslv2 | boost::asio::ssl::context::single_dh_use);
	context_.set_password_callback(boost::bind(&SslServer::get_password, this));
	context_.use_certificate_chain_file("server.pem");
	context_.use_private_key_file("server.pem", boost::asio::ssl::context::pem);
	context_.use_tmp_dh_file("dh512.pem");

	Session* new_session = new Session(io_service_, context_);
	acceptor_.async_accept(new_session->socket(),
							boost::bind(&SslServer::handle_accept, this, new_session, boost::asio::placeholders::error));
}

std::string SslServer::get_password() const
{
	return "test";
}

void SslServer::handle_accept(Session* new_session, const boost::system::error_code& error)
{
	if (!error)
	{
		new_session->start();

		new_session = new Session(io_service_, context_);
		acceptor_.async_accept(new_session->socket(),
								boost::bind(&SslServer::handle_accept, this, new_session, boost::asio::placeholders::error));
	}
	else
	{
		delete new_session;
	}
}
