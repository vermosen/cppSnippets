#pragma once
#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <memory>

#include <boost/thread.hpp>

#include "threadUtil.hpp"

typedef boost::function<void(bool)> connectionDelegate;

class connection
{
public:
	connection(boost::shared_ptr<boost::asio::io_service> io, connectionDelegate cnx)
		: io_(io)
		, st_(*io_)
		, cnx_(cnx)
		, socket_(*io_)
		, resolver_(*io_) {}

	void connect(const std::string & host, int port)
	{
		host_ = host; port_ = port;

		auto query = boost::shared_ptr<boost::asio::ip::tcp::resolver::query>(
			new boost::asio::ip::tcp::resolver::query(
				host_, boost::lexical_cast<std::string>(port)));

		resolver_.async_resolve(*query, st_.wrap(
			boost::bind(&connection::handle_resolve, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::iterator)));
	}

	// accessors
	const std::string & host() const { return host_; }
	int port() const { return port_; }

	boost::shared_ptr<boost::asio::io_service>	io_service() { return io_; };

	boost::asio::strand	&			strand() { return st_; };
	boost::asio::ip::tcp::socket &	socket() { return socket_; };

private:
	void handle_resolve(const boost::system::error_code& err,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	{
		if (!err)
		{
			boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
				st_.wrap(boost::bind(&connection::handle_connect, this,
					boost::asio::placeholders::error)));
		}
		else
		{
			// TODO: error handler
			cnx_(false);
		}
	}
	void handle_connect(const boost::system::error_code& err)
	{
		if (!err)
		{
			cnx_(true);
		}
		else
		{
			// TODO: error handler
			cnx_(false);
		}
	}

	boost::shared_ptr<boost::asio::io_service> io_;

	boost::asio::strand st_;
	boost::asio::ip::tcp::resolver resolver_;
	boost::asio::ip::tcp::socket socket_;

	connectionDelegate cnx_;

	std::string host_;
	int port_;
};

#endif