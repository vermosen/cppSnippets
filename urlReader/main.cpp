//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>

using boost::asio::ip::tcp;

struct connection
{
	connection(const std::string & host, int port)
		: socket_(service_)
		, endpoint_(tcp::resolver(service_).resolve(
			tcp::resolver::query(host, boost::lexical_cast<std::string>(port)))) {}

	tcp::resolver::iterator endpoint_;
	boost::asio::io_service service_;
	tcp::socket socket_;

	void close()
	{
		service_.post(boost::bind(&connection::handle_close, this));
	}
private:
	void handle_connect(const boost::system::error_code& error)
	{
		if (!error)
		{
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.data(), chat_message::header_length),
				boost::bind(&chat_client::handle_read_header, this,
					boost::asio::placeholders::error));
		}
	}
	void handle_close()
	{
		socket_.close();
	}
};

class urlReader
{
public:
	urlReader(connection & cnx)
		: cnx_(cnx)
	{
		boost::asio::async_connect(cnx_.socket_, cnx_.endpoint_,
			boost::bind(&urlReader::handle_connect, this,
				boost::asio::placeholders::error));
	}
	void query(const std::string & url)
	{
		cnx_.service_.post(boost::bind(&urlReader::do_write, this, url));
	}

private:
	void handle_read_header(const boost::system::error_code& error)
	{
		if (!error && read_msg_.decode_header())
		{
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
				boost::bind(&chat_client::handle_read_body, this,
					boost::asio::placeholders::error));
		}
		else
		{
			do_close();
		}
	}
	void handle_read_body(const boost::system::error_code& error)
	{
		if (!error)
		{
			std::cout.write(read_msg_.body(), read_msg_.body_length());
			std::cout << "\n";
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.data(), chat_message::header_length),
				boost::bind(&chat_client::handle_read_header, this,
					boost::asio::placeholders::error));
		}
		else
		{
			do_close();
		}
	}

	void do_write(chat_message msg)
	{
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress)
		{
			boost::asio::async_write(socket_,
				boost::asio::buffer(write_msgs_.front().data(),
					write_msgs_.front().length()),
				boost::bind(&urlReader::handle_write, this,
					boost::asio::placeholders::error));
		}
	}

	void handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			write_msgs_.pop_front();
			if (!write_msgs_.empty())
			{
				boost::asio::async_write(socket_,
					boost::asio::buffer(write_msgs_.front().data(),
						write_msgs_.front().length()),
					boost::bind(&urlReader::handle_write, this,
						boost::asio::placeholders::error));
			}
		}
		else
		{
			do_close();
		}
	}
private:

	std::string readBuf_;
	std::deque<std::string>  Url_;
	connection & cnx_;
};

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: chat_client <host> <port>\n";
			return 1;
		}

		urlReader c(io_service, iterator);

		boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

		char line[chat_message::max_body_length + 1];
		while (std::cin.getline(line, chat_message::max_body_length + 1))
		{
			chat_message msg;
			msg.body_length(std::strlen(line));
			std::memcpy(msg.body(), line, msg.body_length());
			msg.encode_header();
			c.write(msg);
		}

		c.close();
		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}