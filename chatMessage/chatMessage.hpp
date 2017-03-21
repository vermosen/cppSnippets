//
// chat_message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class chat_message
{
public:
	enum { header_length = 4 };
	enum { max_body_length = 512 };

	chat_message();
	const char* data() const;
	char* data();
	size_t length() const;
	const char* body() const;
	char* body();
	size_t body_length() const;
	void body_length(size_t new_length);
	bool decode_header();
	void encode_header();

private:
	char data_[header_length + max_body_length];
	size_t body_length_;
};

#endif // CHAT_MESSAGE_HPP