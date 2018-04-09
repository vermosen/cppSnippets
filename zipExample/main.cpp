#ifndef MAIN_CPP_
#define MAIN_CPP_

#include <exception>
#include <sstream>
#include <iostream>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>

int main(int * argc, char ** argv)
{
	int ret;

	try
	{
		std::stringstream sOut;
		std::stringstream sIn("hello world");

		{
			boost::iostreams::filtering_streambuf<boost::iostreams::output> out(sOut);
			boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
			in.push(boost::iostreams::zlib_compressor());
			in.push(sIn);
			boost::iostreams::copy(in, out);
		}

		sIn.clear(); sIn.seekp(0, std::ios::beg);
		sOut.seekg(0, std::ios::beg);

		{
			boost::iostreams::filtering_streambuf<boost::iostreams::output> out(sIn);
			boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
			in.push(boost::iostreams::zlib_decompressor());
			in.push(sOut);
			boost::iostreams::copy(in, out);
		}

		std::cout << sIn.str() << std::endl;
		ret = 0;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
		ret = 1;
	}

	system("pause");
	return ret;
}

#endif