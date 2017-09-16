#pragma once

#ifndef READER_HPP_
#define READER_HPP_

#include <memory>
#include <array>
#include <utility>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include "connection.hpp"

typedef boost::function<void(const std::string &)> writeDelegate;
typedef boost::function<void(const std::string &, bool)> urlReadDelegate;

class reader
{
public:
	reader(boost::shared_ptr<connection> cnx, urlReadDelegate write)
		: cnx_(cnx)
		, write_(write)
		, transfert_(1) {}

	void reader::getAsync(const std::string & path)
	{
		// build the query
		std::ostream request_stream(&request_);
		request_stream << "GET /";
		request_stream << path;
		request_stream << " HTTP/1.1\r\n";
		request_stream << "Host: " << cnx_->host() << "\r\n";
		request_stream << "Accept: */*\r\n\r\n";

		boost::asio::async_write(cnx_->socket(), request_,
			cnx_->strand().wrap(boost::bind(&reader::handle_write_request, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
	}

private:
	// client callbacks
	void handle_write_request(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			boost::asio::async_read_until(cnx_->socket(), response_, "\r\n",
				cnx_->strand().wrap(boost::bind(&reader::handle_read_status_line, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			write_("", false);
		}
	}
	void handle_read_status_line(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Check that the response is OK.
			std::istream response_stream(&response_);
			std::string http_version;
			response_stream >> http_version;

			unsigned int status_code;
			response_stream >> status_code;

			std::string status_message;
			std::getline(response_stream, status_message);
			if (!response_stream || http_version.substr(0, 5) != "HTTP/")
			{
				write_("", false);
			}

			if (status_code == 200)
			{
				// Read the response headers, which are terminated by a blank line.
				boost::asio::async_read_until(cnx_->socket(), response_, "\r\n\r\n",
					cnx_->strand().wrap(boost::bind(&reader::handle_read_headers, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
			}
			else if (status_code == 302)							// redirection
			{
				// Read the response headers, which are terminated by a blank line.
				boost::asio::async_read_until(cnx_->socket(), response_, "\r\n\r\n",
					cnx_->strand().wrap(boost::bind(&reader::handle_redirection, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
			}
			else
			{
				write_("", false);
			}
		}
		else
		{
			write_("", false);
		}
	}
	void handle_redirection(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Process the response headers.
			std::istream response_stream(&response_);

			// unload the bufstream in header_ until we reached the content (standalone empty line)
			// TODO: we may reach the end of the packet before the end of the header
			std::string h; while (std::getline(response_stream, h))
			{
				std::cout << h << std::endl;
			}
		}
		else
		{
			write_("", false);
		}
	}
	void handle_read_headers(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			// Process the response headers.
			std::istream response_stream(&response_);

			// unload the bufstream in header_ until we reached the content (standalone empty line)
			// TODO: we may reach the end of the packet before the end of the header
			std::string h; while (std::getline(response_stream, h) && h != "\r")
			{
				boost::smatch match;
				
				if (h.find("transfer-encoding: chunked") != std::string::npos) chunked_ = true;
				else if (boost::regex_search(h, match, expr_))
				{
					transfert_ = boost::lexical_cast<int>(match[2]); //TODO: use regex
				}

				header_ << h;
			}

			// Start reading remaining data until EOF.
 			boost::asio::async_read(cnx_->socket(), response_,
				boost::asio::transfer_at_least(transfert_),
				cnx_->strand().wrap(boost::bind(&reader::handle_read_content, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			write_("", false);
		}
	}
	void handle_read_content(const boost::system::error_code& err, size_t bytes_transferred)
	{
		if (!err)
		{
			content_ << &response_;
			write_(content_.str(), true);

			//// Continue reading remaining data until EOF.
			//boost::asio::async_read(cnx_->socket(), response_,
			//	boost::asio::transfer_at_least(1),
			//	cnx_->strand().wrap(boost::bind(&reader::handle_read_content, this,
			//		boost::asio::placeholders::error,
			//		boost::asio::placeholders::bytes_transferred)));
		}
		else if (err != boost::asio::error::eof)
		{
			write_("", false);
		}
	}

private:
	urlReadDelegate write_;
	boost::shared_ptr<connection> cnx_;

	std::string host_;
	int port_;

	std::stringstream content_;
	std::stringstream header_;

	boost::asio::streambuf request_;
	boost::asio::streambuf response_;

	bool ready_;
	bool success_;

	// for chucks management
	bool chunked_;
	int chunckSize_;

	// for size read
	size_t transfert_;
	static const boost::regex expr_;
};

#endif