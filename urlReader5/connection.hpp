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
		, resolver_(*io_) {}

	virtual void connect(const std::string & host, int port) = 0;
	virtual boost::asio::ip::tcp::socket & socket() = 0;

	// accessors
	const std::string & host() const { return host_; }
	int port() const { return port_; }

	boost::shared_ptr<boost::asio::io_service>	io_service() { return io_; };
	boost::asio::strand	&			strand() { return st_; };

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

protected:
	boost::shared_ptr<boost::asio::io_service> io_;

	boost::asio::strand st_;
	boost::asio::ip::tcp::resolver resolver_;

	connectionDelegate cnx_;
	std::string host_;
	int port_;
};

#endif